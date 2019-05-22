#pragma once

#include <cstdint>
#include <vector>

#include "label_parser.h"
#include "v_array.h"
#include "action_score.h"
#include "options.h"


namespace LEARNER {
  template<class T, class E> struct learner;
  using base_learner = learner<char, char>;
}

struct vw;
struct example;
typedef std::vector<example*> multi_ex;

namespace CCB {
  // Each positon in outer array is implicitly the decision corresponding to that index. Each inner array is the result of CB for that call.
  typedef v_array<ACTION_SCORE::action_scores> decision_scores_t;

  struct conditional_contexual_bandit_outcome
  {
    // The cost of this class
    float cost;

    // Either probability for top action or for all actions in action set.
    // Top action is always in first position.
    ACTION_SCORE::action_scores probabilities;
  };

  enum example_type : uint8_t
  {
    unset = 0,
    shared = 1,
    action = 2,
    slot = 3
  };

  struct label {
    example_type type;
    // Outcome may be unset.
    conditional_contexual_bandit_outcome* outcome;
    v_array<uint32_t> explicit_included_actions;
  };

  LEARNER::base_learner* ccb_explore_adf_setup(VW::config::options_i& options, vw& all);

  extern label_parser ccb_label_parser;
}