// Copyright 2018 Open Source Robotics Foundation, Inc.
//
// Licensed under the Apache License, Version 2.0 (the "License");
// you may not use this file except in compliance with the License.
// You may obtain a copy of the License at
//
//     http://www.apache.org/licenses/LICENSE-2.0
//
// Unless required by applicable law or agreed to in writing, software
// distributed under the License is distributed on an "AS IS" BASIS,
// WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
// See the License for the specific language governing permissions and
// limitations under the License.

#include <gtest/gtest.h>

#include <chrono>
#include <string>
#include <memory>

#include "rclcpp/rclcpp.hpp"
#include "rclcpp/exceptions.hpp"
#include "rclcpp/node.hpp"
#include "rclcpp/rclcpp.hpp"
#include "rclcpp/executors.hpp"
#include "rclcpp/executor.hpp"

using namespace std::chrono_literals;

class TestGuardConditionTrigger : public ::testing::Test
{
protected:
  static void SetUpTestCase()
  {
    rclcpp::init(0, nullptr);
  }
};

/*
   Test guard condition trigger is set when a node is added and resetted when it is removed
 */
TEST_F(TestGuardConditionTrigger, set_trigger_guard_condition_multi_threaded) {

  rclcpp::executors::MultiThreadedExecutor executor;

  std::shared_ptr<rclcpp::Node> node =
    std::make_shared<rclcpp::Node>("test_trigger_guard_condtion_multi_threaded");

  auto cbg = node->create_callback_group(rclcpp::CallbackGroupType::Reentrant);

  auto timer_callback = []() {
    printf("Timer executed!");
  };

  auto timer_ = node->create_wall_timer(
      2s, timer_callback, cbg);

  executor.add_node(node);
  ASSERT_EQ(executor.get_guard_condition_trigger(), true);

  executor.remove_node(node);
  ASSERT_EQ(executor.get_guard_condition_trigger(), false);
}

TEST_F(TestGuardConditionTrigger, set_trigger_guard_condition_single_threaded) {

  rclcpp::executors::SingleThreadedExecutor executor;

  std::shared_ptr<rclcpp::Node> node =
    std::make_shared<rclcpp::Node>("test_trigger_guard_condtion_single_threaded");

  auto timer_callback = []() {
    printf("Timer executed!");
  };

  auto timer_ = node->create_wall_timer(
      2s, timer_callback);

  executor.add_node(node);
  ASSERT_EQ(executor.get_guard_condition_trigger(), false);

  executor.remove_node(node);
  ASSERT_EQ(executor.get_guard_condition_trigger(), false);
}
