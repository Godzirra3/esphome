#include "dfrobot_sen0623_button.h"

namespace esphome {
namespace dfrobot_sen0623 {

void Sen0623ResetButton::press_action() {
   this->parent_->cmd_reset();
}
}  // namespace dfrobot_sen0623
}  // namespace esphome
