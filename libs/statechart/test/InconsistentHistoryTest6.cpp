//////////////////////////////////////////////////////////////////////////////
// (c) Copyright Andreas Huber Doenni 2005
// Distributed under the Boost Software License, Version 1.0. (See accompany-
// ing file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//////////////////////////////////////////////////////////////////////////////



#include <boost/fsm/state_machine.hpp>
#include <boost/fsm/simple_state.hpp>
#include <boost/fsm/event.hpp>
#include <boost/fsm/custom_reaction.hpp>



namespace fsm = boost::fsm;



struct EvX : fsm::event< EvX > {};

struct A;
struct InconsistentHistoryTest : fsm::state_machine<
  InconsistentHistoryTest, A > {};

struct B;
// A only has deep history
struct A : fsm::simple_state< A, InconsistentHistoryTest,
  fsm::custom_reaction< EvX >, B, fsm::has_deep_history >
{
  fsm::result react( const EvX & )
  {
    // A only has deep history
    clear_shallow_history< A, 0 >();
    return discard_event();
  }
};

  struct B : fsm::simple_state< B, A > {};


int main()
{
  InconsistentHistoryTest machine;
  machine.initiate();
  return 0;
}
