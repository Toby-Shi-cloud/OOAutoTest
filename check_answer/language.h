#ifndef _LANGUAGE_H
#define _LANGUAGE_H

#define FORE_RED    "\x1b[31m"
#define FORE_RESET  "\x1b[39m"

#ifdef _LANGUAGE_ENGLISH

#define ARRIVE_TOO_MANY_FLOORS "arrive: Moved More Than One Floor" 
#define ARRIVE_NO_SUCH_FLOOR "arrive: No Such Floor"
#define ARRIVE_TOO_HURRY "arrive: Moved Too Hurry"
#define ARRIVE_NOT_CLOSED "arrive: Door Left Open"
#define ARRIVE_TOO_MANY_TIMES "arrive: Please Maintain Elevator"

#define OPEN_WRONG_FLOOR "open: Elev Not At This Floor"
#define OPEN_TWICE "open: Door Already Opened"
#define OPEN_TOO_HURRY "open: Elev Moving Or Closing"

#define CLOSE_WRONG_FLOOR "close: Elev Not At This Floor"
#define CLOSE_TWICE "close: Door Already Closed"
#define CLOSE_TOO_HURRY "close: Close Too Hurry"

#define IN_ENTER_TWICE "in: Passenger Entering Perfectly, Twice"
#define IN_WRONG_FLOOR "in: Passenger Getting Into Elev Without Elev"
#define IN_FULL_CLOSED "in: Elev Full Or Closed"

#define OUT_EXIT_TWICE "out: Passenger Exit From A Floor"
#define OUT_WRONG_ELEV "out: Passenger Exit In Another Elev"
#define OUT_ELEV_CLOSED "out: Elevator Closed"

#define MAINTAIN_BAD_STATE "maintain: Bad State"
#define MAINTAIN_NOT_ACCEPTED "maintain: No Maintain Request"
#define MAINTAIN_NOT_CLOSED "maintain: Door Left Open"
#define MAINTAIN_NOT_EMPTY "maintain: Elevator Not Empty"

#define WRONG_TIME_ORDER "Wrong Time Flow"
#define NO_ELEVATOR "No Such Elevator"
#define NO_PASSENGER "No Such Passenger"
#define DUPLICATE_ELEVATOR "Duplicate Elevator ID"
#define DUPLICATE_PASSENGER "Duplicate Passenger ID"
#define UNKNOWN_ACTION "Sorry, I dont understand"
#define UNKNOWN_FORMAT_INPUT "Bad Input Format"
#define UNKNOWN_FORMAT_OUTPUT "Bad Output Format"

#define PASSENGER_TRAPPED(id) ("Passenger Left On Elev: " FORE_RED "Passenger(" + std::to_string(id) + ")" FORE_RESET)
#define PASSENGER_WRONG_DIST(id) ("Passenger Not At Destination: " FORE_RED "Passenger(" + std::to_string(id) + ")" FORE_RESET)
#define ELEVATOR_NOT_CLOSED(id) ("Door Left Open: " FORE_RED "Elevator(" + std::to_string(id) + ")" FORE_RESET)

#else

#define ARRIVE_TOO_MANY_FLOORS "arrive: 移动了超过一层"
#define ARRIVE_NO_SUCH_FLOOR "arrive: 移动到不存在的楼层"
#define ARRIVE_TOO_HURRY "arrive: 移动时间不足"
#define ARRIVE_NOT_CLOSED "arrive: 电梯门未关闭"
#define ARRIVE_TOO_MANY_TIMES "arrive: 请进行电梯维护"

#define OPEN_WRONG_FLOOR "open: 电梯不在该楼层"
#define OPEN_TWICE "open: 电梯门已经打开"
#define OPEN_TOO_HURRY "open: 电梯正在移动/关闭"

#define CLOSE_WRONG_FLOOR "close: 电梯不在该楼层"
#define CLOSE_TWICE "close: 电梯门已经关闭"
#define CLOSE_TOO_HURRY "close: 关闭时间不足"

#define IN_ENTER_TWICE "in: 乘客已经进入电梯"
#define IN_WRONG_FLOOR "in: 乘客不在电梯所在楼层"
#define IN_FULL_CLOSED "in: 电梯已满或门已关闭"

#define OUT_EXIT_TWICE "out: 乘客已经离开电梯"
#define OUT_WRONG_ELEV "out: 乘客不在该电梯"
#define OUT_ELEV_CLOSED "out: 电梯门已关闭"

#define MAINTAIN_BAD_STATE "maintain: 错误维护状态"
#define MAINTAIN_NOT_ACCEPTED "maintain: 未接受维护请求"
#define MAINTAIN_NOT_CLOSED "maintain: 电梯门未关闭"
#define MAINTAIN_NOT_EMPTY "maintain: 电梯不为空"

#define WRONG_TIME_ORDER "时间不是递增的"
#define NO_ELEVATOR "该电梯不存在"
#define NO_PASSENGER "该乘客不存在"
#define DUPLICATE_ELEVATOR "电梯 ID 重复"
#define DUPLICATE_PASSENGER "乘客 ID 重复"
#define UNKNOWN_ACTION "未知事件"
#define UNKNOWN_FORMAT_INPUT "输入格式错误"
#define UNKNOWN_FORMAT_OUTPUT "输出格式错误"

#define PASSENGER_TRAPPED(id) ("乘客被困在电梯中: " FORE_RED "乘客 ID = " + std::to_string(id) + FORE_RESET)
#define PASSENGER_WRONG_DIST(id) ("乘客前往错误楼层: " FORE_RED "乘客 ID = " + std::to_string(id) + FORE_RESET)
#define ELEVATOR_NOT_CLOSED(id) ("电梯门未关: " FORE_RED "电梯 ID = " + std::to_string(id) + FORE_RESET)

#endif

#endif