#include "logic_commands.hpp"

LogicCommands::LogicCommands(Console& console, Settings& settings, Game& game)
    : console_(&console)
    , settings_(&settings)
    , game_(&game)
{}

// code
void LogicCommands::errorCommand() {
    console_->SendError("command not found");
}

void LogicCommands::sayPong() {
    console_->SendConsole("pong");
}

void LogicCommands::exitProgram() {
    console_->SendConsole("ok");
    exit(0); // завершение без ошибки
}

void LogicCommands::createGame(std::string mode) {
    if (mode == "master") {
        settings_->mode_ = 0;
        settings_->size_x_ = 10;
        settings_->size_y_ = 10;
        delete[] settings_->count_ships_;
        settings_->count_ships_ = new uint64_t[4]{4, 3, 2, 1};

        console_->SendConsole("ok");
    } else if (mode == "slave") {
        settings_->mode_ = 1;
        settings_->size_x_ = 0;
        settings_->size_y_ = 0;
        delete[] settings_->count_ships_;
        settings_->count_ships_ = new uint64_t[4]{0, 0, 0, 0};

        console_->SendConsole("ok");
    } else {
        console_->SendError("there is no mode");
    }
}

// set
void LogicCommands::setHeight(uint64_t height) {
    if (!settings_->is_running_) {
        settings_->size_y_ = height;
        console_->SendConsole("ok");
    } else {
        console_->SendConsole("failed");
    }
}

void LogicCommands::setWidth(uint64_t width) {
    if (!settings_->is_running_) {
        settings_->size_x_ = width;
        console_->SendConsole("ok");
    } else {
        console_->SendConsole("failed");
    }
}

void LogicCommands::setCount(uint64_t ship, uint64_t count) {
    if (!settings_->is_running_) {
        settings_->count_ships_[ship - 1] = count;
        console_->SendConsole("ok");
    } else {
        console_->SendConsole("failed");
    }
}

void LogicCommands::setStrategy(std::string q) {
    if (q == "ordered") {
        settings_->strategy_ = 0;
        console_->SendConsole("ok");
    } else if (q == "custom") {
        settings_->strategy_ = 1;
        console_->SendConsole("ok");
    } else {
        console_->SendError("there is no such strategy.");
    }
}

// get
void LogicCommands::getHeight() {
    console_->SendConsole(settings_->size_y_);
}

void LogicCommands::getWidth() {
    console_->SendConsole(settings_->size_x_);
}

void LogicCommands::getCount(uint64_t ship) {
    console_->SendConsole(settings_->count_ships_[ship - 1]);
}

// other
void LogicCommands::isFinished() {
    if (settings_->win_ != -1) {
        console_->SendConsole("yes");
    } else {
        console_->SendConsole("no");
    }
}

void LogicCommands::amIWin() {
    if (settings_->win_ == 0) {
        console_->SendConsole("yes");
    } else {
        console_->SendConsole("no");
    }
}

void LogicCommands::amILose() {
    if (settings_->win_ == 1) {
        console_->SendConsole("yes");
    } else {
        console_->SendConsole("no");
    }
}

// do
void LogicCommands::doCommand(Arguments* arg) {
    if (arg->error_) {
        errorCommand();
    } else if (arg->com_ == ping) {
        sayPong();
    } else if (arg->com_ == exitt) {
        exitProgram();
    } else if (arg->com_ == create) {
        createGame(arg->string_args_[0]);
    } else if (arg->com_ == start) {
        game_->startGame();
    } else if (arg->com_ == stop) {
        game_->stopGame();
    } else if (arg->com_ == set) {
        if (arg->string_args_[0] == "height") {
            setHeight(arg->int_args_[0]);
        } else if (arg->string_args_[0] == "width") {
            setWidth(arg->int_args_[0]);
        } else if (arg->string_args_[0] == "count") {
            setCount(arg->int_args_[0], arg->int_args_[1]);
        } else if (arg->string_args_[0] == "strategy") {
            setStrategy(arg->string_args_[1]);
        } else if (arg->string_args_[0] == "result") {
            game_->setResult(arg->string_args_[1]);
        }
    } else if (arg->com_ == get) {
        if (arg->string_args_[0] == "height") {
            getHeight();
        } else if (arg->string_args_[0] == "width") {
            getWidth();
        } else if (arg->string_args_[0] == "count") {
            getCount(arg->int_args_[0]);
        }
    } else if (arg->com_ == shot) {
        if (arg->int_args_.size() != 0) {
            game_->shotIn(arg->int_args_[0], arg->int_args_[1]);
        } else {
            game_->shotOut();
        }
    } else if (arg->com_ == finished) {
        isFinished();
    } else if (arg->com_ == win) {
        amIWin();
    } else if (arg->com_ == lose) {
        amILose();
    } else if (arg->com_ == dump) {
        game_->dumpFile(arg->string_args_[0]);
    } else if (arg->com_ == load) {
        game_->loadFile(arg->string_args_[0]);
    }

    delete arg;
}