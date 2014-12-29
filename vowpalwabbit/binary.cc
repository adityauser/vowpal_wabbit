#include <float.h>
#include "reductions.h"
#include "multiclass.h"
#include "simple_label.h"

namespace BINARY {
  template <bool is_learn>
  void predict_or_learn(char&, LEARNER::base_learner& base, example& ec) {
    if (is_learn)
      base.learn(ec);
    else
      base.predict(ec);

    if ( ec.pred.scalar > 0)
      ec.pred.scalar = 1;
    else
      ec.pred.scalar = -1;

    if (ec.l.simple.label != FLT_MAX)
      {
	if (fabs(ec.l.simple.label) != 1.f)
	  cout << "You are using a label not -1 or 1 with a loss function expecting that!" << endl;
	else
	  if (ec.l.simple.label == ec.pred.scalar)
	    ec.loss = 0.;
	  else
	    ec.loss = ec.l.simple.weight; 
      }
  }

LEARNER::base_learner* setup(vw& all, po::variables_map& vm)
  {//parse and set arguments
    po::options_description opts("Binary options");
    opts.add_options()
      ("binary", "report loss as binary classification on -1,1");
    vm = add_options(all,opts);
    if(!vm.count("binary"))
      return NULL;

    //Create new learner
    LEARNER::learner<char>& ret = 
      LEARNER::init_learner<char>(NULL, all.l, predict_or_learn<true>, predict_or_learn<false>);
    return make_base(ret);
  }
}
