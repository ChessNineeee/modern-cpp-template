//
// Created by wuzhengqi on 24-11-30.
//

#include <gtest/gtest.h>

#include <format>

struct TcpEvent_Open
{
};
struct TcpEvent_Listen
{
};
struct TcpEvent_Recv_Syn_And_Ack
{
};
struct TcpEvent_Recv_Syn
{
};
struct TcpEvent_Close
{
};
struct TcpEvent_Recv_Ack
{
};
struct TcpEvent_Recv_Fin
{
};
struct TcpEvent_Two_Msl
{

};
using TcpEvent =
    std::variant<TcpEvent_Open, TcpEvent_Listen, TcpEvent_Recv_Syn, TcpEvent_Recv_Syn_And_Ack, TcpEvent_Close, TcpEvent_Recv_Ack, TcpEvent_Recv_Fin, TcpEvent_Two_Msl>;

struct TcpState_Closed
{
};
template<>
struct std::formatter<TcpState_Closed> : std::formatter<std::string>
{
  auto format(TcpState_Closed _p, format_context& ctx) const
  {
    return formatter<string>::format(std::format("{}", "CLOSED"), ctx);
  }
} __attribute__((packed));

struct TcpState_Listen
{
};
template<>
struct std::formatter<TcpState_Listen> : std::formatter<std::string>
{
  auto format(TcpState_Listen _p, format_context& ctx) const
  {
    return formatter<string>::format(std::format("{}", "LISTEN"), ctx);
  }
} __attribute__((packed));

struct TcpState_Rcvd
{
};
template<>
struct std::formatter<TcpState_Rcvd> : std::formatter<std::string>
{
  auto format(TcpState_Rcvd _p, format_context& ctx) const
  {
    return formatter<string>::format(std::format("{}", "SYN_RCVD"), ctx);
  }
} __attribute__((packed));

struct TcpState_Syn_Sent
{
};
template<>
struct std::formatter<TcpState_Syn_Sent> : std::formatter<std::string>
{
  auto format(TcpState_Syn_Sent _p, format_context& ctx) const
  {
    return formatter<string>::format(std::format("{}", "SYN_SENT"), ctx);
  }
} __attribute__((packed));

struct TcpState_Established
{
};
template<>
struct std::formatter<TcpState_Established> : std::formatter<std::string>
{
  auto format(TcpState_Established _p, format_context& ctx) const
  {
    return formatter<string>::format(std::format("{}", "ESTABLISHED"), ctx);
  }
} __attribute__((packed));

struct TcpState_Close_Wait
{
};
template<>
struct std::formatter<TcpState_Close_Wait> : std::formatter<std::string>
{
  auto format(TcpState_Close_Wait _p, format_context& ctx) const
  {
    return formatter<string>::format(std::format("{}", "CLOSE_WAIT"), ctx);
  }
} __attribute__((packed));

struct TcpState_Last_Ack
{
};
template<>
struct std::formatter<TcpState_Last_Ack> : std::formatter<std::string>
{
  auto format(TcpState_Last_Ack _p, format_context& ctx) const
  {
    return formatter<string>::format(std::format("{}", "LAST_ACK"), ctx);
  }
} __attribute__((packed));

struct TcpState_Fin_Wait_1
{
};
template<>
struct std::formatter<TcpState_Fin_Wait_1> : std::formatter<std::string>
{
  auto format(TcpState_Fin_Wait_1 _p, format_context& ctx) const
  {
    return formatter<string>::format(std::format("{}", "FIN_WAIT_1"), ctx);
  }
} __attribute__((packed));

struct TcpState_Fin_Wait_2
{
};
template<>
struct std::formatter<TcpState_Fin_Wait_2> : std::formatter<std::string>
{
  auto format(TcpState_Fin_Wait_2 _p, format_context& ctx) const
  {
    return formatter<string>::format(std::format("{}", "FIN_WAIT_2"), ctx);
  }
} __attribute__((packed));

struct TcpState_Time_Wait
{
};
template<>
struct std::formatter<TcpState_Time_Wait> : std::formatter<std::string>
{
  auto format(TcpState_Time_Wait _p, format_context& ctx) const
  {
    return formatter<string>::format(std::format("{}", "TIME_WAIT"), ctx);
  }
} __attribute__((packed));

using TcpState =
    std::variant<TcpState_Closed, TcpState_Syn_Sent, TcpState_Established, TcpState_Fin_Wait_1, TcpState_Fin_Wait_2, TcpState_Time_Wait, TcpState_Listen, TcpState_Rcvd, TcpState_Close_Wait, TcpState_Last_Ack>;
struct TcpStateFormatVisitor
{
  template<typename StateParam>
  std::string operator()(StateParam const& s)
  {
    return std::format("{}", s);
  }
};
template<>
struct std::formatter<TcpState> : std::formatter<std::string>
{
  auto format(TcpState s, format_context& ctx) const
  {
    return formatter<string>::format(std::visit(TcpStateFormatVisitor{}, s), ctx);
  }
} __attribute__((packed));

struct TcpStateTransition
{
  std::optional<TcpState> operator()(TcpState_Closed const& s, TcpEvent_Open const& e)
  {
    return TcpState_Syn_Sent{};
  }

  std::optional<TcpState> operator()(TcpState_Closed const& s, TcpEvent_Listen const& e)
  {
    return TcpState_Listen{};
  }

  std::optional<TcpState> operator()(TcpState_Listen const& s, TcpEvent_Recv_Syn const& e)
  {
    return TcpState_Rcvd{};
  }

  std::optional<TcpState> operator()(TcpState_Rcvd const& s, TcpEvent_Recv_Ack const& e)
  {
    return TcpState_Established{};
  }

  std::optional<TcpState> operator()(TcpState_Syn_Sent const& s, TcpEvent_Recv_Syn_And_Ack const& e)
  {
    return TcpState_Established{};
  }

  std::optional<TcpState> operator()(TcpState_Established const& s, TcpEvent_Close const& e)
  {
    return TcpState_Fin_Wait_1{};
  }

  std::optional<TcpState> operator()(TcpState_Established const& s, TcpEvent_Recv_Fin const& e)
  {
    return TcpState_Close_Wait{};
  }

  std::optional<TcpState> operator()(TcpState_Close_Wait const& s, TcpEvent_Close const& e)
  {
    return TcpState_Last_Ack{};
  }

  std::optional<TcpState> operator()(TcpState_Last_Ack const& s, TcpEvent_Recv_Ack const& e)
  {
    return TcpState_Closed{};
  }

  std::optional<TcpState> operator()(TcpState_Fin_Wait_1 const& s, TcpEvent_Recv_Ack const& e)
  {
    return TcpState_Fin_Wait_2{};
  }

  std::optional<TcpState> operator()(TcpState_Fin_Wait_2 const& s, TcpEvent_Recv_Fin const& e)
  {
    return TcpState_Time_Wait{};
  }

  std::optional<TcpState> operator()(TcpState_Time_Wait const& s, TcpEvent_Two_Msl const& e)
  {
    return TcpState_Closed{};
  }
  template<typename StateParam, typename EventParam>
  std::optional<TcpState> operator()(StateParam const& s, EventParam const& e)
  {
    return std::nullopt;
  }
};

class TcpConnectionStateMachine
{
 public:
  void TryUpdateConnectionState(TcpEvent const& e)
  {
    if (auto nextState = std::visit(TcpStateTransition{}, state_, e))
    {
      state_ = nextState.value();
    }
  }

  [[nodiscard]] std::string State() const
  {
    return std::format("{}", state_);
  }

 private:
  TcpState state_ = TcpState_Closed{};
};

TEST(TcpConnectionState, ClientConnectionHappyPath)
{
  auto fsm = TcpConnectionStateMachine{};
  ASSERT_EQ(fsm.State(), "CLOSED");

  auto open = TcpEvent_Open{};
  fsm.TryUpdateConnectionState(open);
  ASSERT_EQ(fsm.State(), "SYN_SENT");

  auto recv_syn_and_ack = TcpEvent_Recv_Syn_And_Ack{};
  fsm.TryUpdateConnectionState(recv_syn_and_ack);
  ASSERT_EQ(fsm.State(), "ESTABLISHED");

  auto close = TcpEvent_Close{};
  fsm.TryUpdateConnectionState(close);
  ASSERT_EQ(fsm.State(), "FIN_WAIT_1");

  auto recv_ack = TcpEvent_Recv_Ack{};
  fsm.TryUpdateConnectionState(recv_ack);
  ASSERT_EQ(fsm.State(), "FIN_WAIT_2");

  auto recv_fin = TcpEvent_Recv_Fin{};
  fsm.TryUpdateConnectionState(recv_fin);
  ASSERT_EQ(fsm.State(), "TIME_WAIT");

  auto two_msl = TcpEvent_Two_Msl{};
  fsm.TryUpdateConnectionState(two_msl);
  ASSERT_EQ(fsm.State(), "CLOSED");
}

TEST(TcpConnectionState, ServerConnectionHappyPath)
{
  auto fsm = TcpConnectionStateMachine{};
  ASSERT_EQ(fsm.State(), "CLOSED");

  auto listen = TcpEvent_Listen{};
  fsm.TryUpdateConnectionState(listen);
  ASSERT_EQ(fsm.State(), "LISTEN");

  auto recv_syn = TcpEvent_Recv_Syn{};
  fsm.TryUpdateConnectionState(recv_syn);
  ASSERT_EQ(fsm.State(), "SYN_RCVD");

  auto recv_ack = TcpEvent_Recv_Ack{};
  fsm.TryUpdateConnectionState(recv_ack);
  ASSERT_EQ(fsm.State(), "ESTABLISHED");

  auto recv_fin = TcpEvent_Recv_Fin{};
  fsm.TryUpdateConnectionState(recv_fin);
  ASSERT_EQ(fsm.State(), "CLOSE_WAIT");

  auto close = TcpEvent_Close{};
  fsm.TryUpdateConnectionState(close);
  ASSERT_EQ(fsm.State(), "LAST_ACK");

  auto two_msl = TcpEvent_Recv_Ack{};
  fsm.TryUpdateConnectionState(two_msl);
  ASSERT_EQ(fsm.State(), "CLOSED");
}

int main(int argc, char** argv)
{
  ::testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}