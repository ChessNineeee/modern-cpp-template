#include <gtest/gtest.h>

#include <format>
#include <iostream>
#include <optional>
#include <variant>
//
// Created by wuzhengqi on 24-11-29.
//

struct Event_Connect { std::string_view address; };
template<>
struct std::formatter<Event_Connect> : std::formatter<std::string> {
  auto format(Event_Connect p, format_context& ctx) const {
    return formatter<string>::format(
      std::format("[{}]", "Event_Connect"), ctx);
  }
};
struct Event_Connected {};
template<>
struct std::formatter<Event_Connected> : std::formatter<std::string> {
  auto format(Event_Connected p, format_context& ctx) const {
    return formatter<string>::format(
      std::format("[{}]", "Event_Connected"), ctx);
  }
};
struct Event_Disconnect {};
template<>
struct std::formatter<Event_Disconnect> : std::formatter<std::string> {
  auto format(Event_Disconnect p, format_context& ctx) const {
    return formatter<string>::format(
      std::format("[{}]", "Event_Disconnect"), ctx);
  }
};

struct Event_Timeout {};
template<>
struct std::formatter<Event_Timeout> : std::formatter<std::string> {
  auto format(Event_Timeout p, format_context& ctx) const {
    return formatter<string>::format(
      std::format("[{}]", "Event_Timeout"), ctx);
  }
};

using Event = std::variant<Event_Connect, Event_Disconnect, Event_Timeout, Event_Connected>;
template<>
struct std::formatter<Event> : std::formatter<std::string> {
  auto format(Event p, format_context& ctx) const {
    return formatter<string>::format(
      std::format("[{}]", p.index()), ctx);
  }
};

struct State_Idle{};
template<>
struct std::formatter<State_Idle> : std::formatter<std::string> {
  auto format(State_Idle p, format_context& ctx) const {
    return formatter<string>::format(
      std::format("[{}]", "State_Idle"), ctx);
  }
};

struct State_Connecting
{
  static constexpr int n_max = 3;
  int n = 0;
  std::string address;
};
template<>
struct std::formatter<State_Connecting> : std::formatter<std::string> {
  auto format(State_Connecting p, format_context& ctx) const {
    return formatter<string>::format(
      std::format("[{} {} {}]", "State_Connecting", p.n, p.address), ctx);
  }
};

struct State_Connected{};
template <>
struct std::formatter<State_Connected> : std::formatter<std::string> {
  auto format(State_Connected p, format_context& ctx) const {
    return formatter<string>::format(
      std::format("[{}]", "State_Connected"), ctx);
  }
};
using State = std::variant<State_Idle, State_Connecting, State_Connected>;

template <>
struct std::formatter<State> : std::formatter<std::string> {
  auto format(State p, format_context& ctx) const {
    return formatter<string>::format(
      std::format("[{}]", p.index()), ctx);
  }
};

struct Transitions
{
  std::optional<State> operator() (State_Idle&, const Event_Connect& e)
  {
    return State{State_Connecting {
      .address = std::string(e.address)
    }};
  }
  std::optional<State> operator() (State_Connecting&, const Event_Connected& e)
  {
    return State{State_Connected{}};
  }
  std::optional<State> operator() (State_Connected&, const Event_Disconnect& e)
  {
    return State{State_Idle{}};
  }
  std::optional<State> operator() (State_Connecting& s, const Event_Timeout& e)
  {
    return s.n < s.n_max ? State{State_Connecting{.address = s.address, .n = s.n + 1, }} : State{State_Idle{}};
  }
  // fallback
  template<typename S, typename E>
  std::optional<State> operator() (S& s, const E& e)
  {
    std::cout << std::format("unknown transition, state: {}, event: {}", s, e) << '\n';
    return std::nullopt;
  }
};

struct StateMachine
{
  State state_ = State_Idle{};
  void tryUpdateState(const Event& e)
  {
    if (auto nextState = std::visit(Transitions{}, state_, e))
    {
      state_ = nextState.value();
    }
  }
};

TEST(StateMachine, tryUpdateState)
{
  auto fsm = StateMachine{};
  auto connect = Event{Event_Connect{.address = "127.0.0.1"}};

  fsm.tryUpdateState(connect);
  ASSERT_EQ(fsm.state_.index(), 1);

  auto disconnect = Event{Event_Disconnect{}};
  fsm.tryUpdateState(disconnect);
  ASSERT_EQ(fsm.state_.index(), 1);

  auto timeout = Event_Timeout{};
  fsm.tryUpdateState(timeout);
  ASSERT_EQ(fsm.state_.index(), 1); // n = 1

  fsm.tryUpdateState(timeout);
  ASSERT_EQ(fsm.state_.index(), 1); // n = 2

  fsm.tryUpdateState(timeout);
  ASSERT_EQ(fsm.state_.index(), 1); // n = 3

  fsm.tryUpdateState(timeout);
  ASSERT_EQ(fsm.state_.index(), 0);

  fsm.tryUpdateState(connect);
  ASSERT_EQ(fsm.state_.index(), 1);

  auto connected = Event{Event_Connected{}};
  fsm.tryUpdateState(connected);
  ASSERT_EQ(fsm.state_.index(), 2);

  fsm.tryUpdateState(disconnect);
  ASSERT_EQ(fsm.state_.index(), 0);

  fsm.tryUpdateState(connected);
  ASSERT_EQ(fsm.state_.index(), 0);
}

int main(int argc, char **argv)
{
  ::testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}