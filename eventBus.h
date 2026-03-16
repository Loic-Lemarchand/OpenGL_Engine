#pragma once

#include <string>
#include <vector>
#include <functional>
#include <unordered_map>
#include <memory>
#include <tuple>
#include <type_traits>

namespace EventDispatcher
{
	#define BIT(x) (1 << x)

	enum EventType
	{
		None = 0,
		KeyPressed = BIT(0),
		KeyReleased = BIT(1),
		WindowResized = BIT(2),
		RotateLeft = BIT(3),
		RotateRight = BIT(4),
		MouseMoved = BIT(5)
	};

	class Event
	{
	public:
		Event(EventType type, std::string name) : myType(type), myName(name)  {}
		virtual ~Event() = default;

		virtual EventType getType() const { return myType;  }
		virtual std::string getName() const { return myName;  }

		virtual int getCategoryFlags() const = 0;

		bool isInCategory(EventType category)
		{
			return static_cast<int>(getCategoryFlags()) & static_cast<int>(category);
		}

	private:

		EventType myType;

		std::string myName;

	};

	//Generic event
	template<typename... Args>
	class PayloadEvent : public Event
	{
	public:
		using Tuple = std::tuple<std::decay_t<Args>...>;

		template<typename... Ts>
		PayloadEvent(EventType type, std::string name, Ts&&... args)
			: Event(type, std::move(name)), myData(std::forward<Ts>(args)...) {
		}

		const Tuple& data() const { return myData; }

		template<std::size_t I>
		const auto& get() const { return std::get<I>(myData); }

		// For compatibility purpose, TODO : REMOVE
		int getCategoryFlags() const override { return static_cast<int>(getType()); }
	private:
		Tuple myData;
	};

	template<typename... Args>
	std::unique_ptr<Event> makeEvent(EventType type, std::string name, Args&&... args)
	{
		return std::make_unique<PayloadEvent<std::decay_t<Args>...>>(type, std::move(name), std::forward<Args>(args)...);
	}

	class EventBus
	{
	public:
		EventBus();
		~EventBus();

		void pushEvent(std::unique_ptr<Event> event);

		void dispatchEvents();

		void clearEvents();

		// generic subscribe which can be used with lambda
		void subscribe(EventType type, std::function<void(const Event&)> callback);

		// --- New overloads: méthodes membres (raw pointer) ---
		template<typename T>
		void subscribe(EventType type, void (T::* method)(const Event&), T* instance)
		{
			// wrap into the existing std::function subscription
			subscribe(type, [method, instance](const Event& e) {
				(instance->*method)(e);
				});
		}

		// payload-unpacking: member function taking the payload args
		template<typename T, typename... Args>
		void subscribe(EventType type, void (T::* method)(Args...), T* instance)
		{
			subscribe(type, [method, instance](const Event& e) {
				using ExpectedPayload = PayloadEvent<Args...>;
				if (const ExpectedPayload* payload = dynamic_cast<const ExpectedPayload*>(&e))
				{
					std::apply([method, instance](const auto&... unpacked) {
						(instance->*method)(unpacked...);
						}, payload->data());
				}
				else
				{
					// warning when payload type does not match
					std::fprintf(stderr, "Warning: Event payload mismatch for EventType %d. Expected '%s', got '%s'.\n",
						static_cast<int>(e.getType()),
						typeid(e).name(),
						typeid(ExpectedPayload).name());
				}
				});
		}

		// --- Versions sécurisées avec shared_ptr (stocke un weak_ptr interne) ---
		template<typename T>
		void subscribe(EventType type, void (T::* method)(const Event&), const std::shared_ptr<T>& instance)
		{
			std::weak_ptr<T> wp = instance;
			subscribe(type, [method, wp](const Event& e) {
				if (auto sp = wp.lock()) (sp.get()->*method)(e);
				});
		}

		template<typename T, typename... Args>
		void subscribe(EventType type, void (T::* method)(Args...), const std::shared_ptr<T>& instance)
		{
			std::weak_ptr<T> wp = instance;
			subscribe(type, [method, wp](const Event& e) {
				if (auto* payload = dynamic_cast<const PayloadEvent<Args...>*>(&e))
				{
					if (auto sp = wp.lock())
					{
						std::apply([method, sp](const auto&... unpacked) {
							(sp.get()->*method)(unpacked...);
							}, payload->data());
					}
				}
				});
		}

	private:

		std::vector<std::unique_ptr<Event>> myEventQueue;

		std::unordered_map<EventType, std::vector<std::function<void(const Event&)>>> mySubscribers;
	};

	

}