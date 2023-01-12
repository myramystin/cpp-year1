#include "minesweeper.h"
#include <algorithm>
#include <deque>
#include <ctime>

Minesweeper::RectAround Minesweeper::GetRectAround(size_t x, size_t y) {
    RectAround result;
    result.y_begin = 0U;
    if (y != 0U) {
        result.y_begin = y - 1U;
    }
    result.x_begin = 0U;
    if (x != 0U) {
        result.x_begin = x - 1U;
    }
    if (y == height_ - 1U) {
        result.y_end = height_;
    } else {
        result.y_end = y + 2U;
    }
    if (x == width_ - 1U) {
        result.x_end = width_;
    } else {
        result.x_end = x + 2U;
    }
    return result;
}

void Minesweeper::EmptyField() {
    for (size_t i = 0U; i < height_; ++i) {
        std::vector<CellWithCounters> cell_line(width_);
        for (size_t j = 0U; j < width_; ++j) {
            CellWithCounters current_cell;
            current_cell.x = j;
            current_cell.y = i;
            cell_line[j] = current_cell;
        }
        field_.push_back(cell_line);
    }
}

void Minesweeper::IncNeighborsBombsCount(size_t x, size_t y) {
    auto cycle_limits = GetRectAround(x, y);
    for (size_t i = cycle_limits.y_begin; i < cycle_limits.y_end; ++i) {
        for (size_t j = cycle_limits.x_begin; j < cycle_limits.x_end; ++j) {
            if (!(i == y && j == x) && field_[i][j].status != Minesweeper::CellStatus::IS_BOMB) {
                ++field_[i][j].bombs_count;
                field_[i][j].status = CellStatus::IS_BOMB_NEIGHBOR;
            }
        }
    }
}

Minesweeper::Minesweeper(size_t width, size_t height, const std::vector<Cell>& cells_with_mines) {
    NewGame(width, height, cells_with_mines);
}

Minesweeper::Minesweeper(size_t width, size_t height, size_t mines_count) {
    NewGame(width, height, mines_count);
}

Minesweeper::RenderedField Minesweeper::RenderField() const {
    Minesweeper::RenderedField result;
    for (size_t i = 0U; i < height_; ++i) {
        std::string line;
        for (size_t j = 0U; j < width_; ++j) {
            if (field_[i][j].is_flag) {
                line += '?';
            } else {
                if (field_[i][j].visibility == CellVisibilityStatus::IS_OPEN) {
                    const auto& current_status = field_[i][j].status;
                    if (current_status == CellStatus::IS_EMPTY) {
                        line += '.';
                    } else if (current_status == CellStatus::IS_BOMB_NEIGHBOR) {
                        line += '0' + field_[i][j].bombs_count;
                    } else {
                        line += '*';
                    }
                } else {
                    line += '-';
                }
            }
        }
        result.push_back(line);
    }
    return result;
}

void Minesweeper::MarkCell(const Cell& cell) {
    field_[cell.y][cell.x].is_flag = !field_[cell.y][cell.x].is_flag;
}

void Minesweeper::OpenAll() {
    for (auto& line : field_) {
        for (auto& cell : line) {
            cell.visibility = CellVisibilityStatus::IS_OPEN;
        }
    }
}

void Minesweeper::OpenCell(const Cell& cell) {
    if (status_ == GameStatus::NOT_STARTED) {
        status_ = GameStatus::IN_PROGRESS;
        game_start_time_ = std::time(nullptr);
    }
    if (status_ == GameStatus::IN_PROGRESS) {
        if (field_[cell.y][cell.x].status == CellStatus::IS_BOMB) {
            status_ = GameStatus::DEFEAT;
            OpenAll();
        } else if (field_[cell.y][cell.x].status == CellStatus::IS_BOMB_NEIGHBOR) {
            field_[cell.y][cell.x].visibility = CellVisibilityStatus::IS_OPEN;
        } else {
            std::deque<std::pair<size_t, size_t>> check_queue;
            check_queue.push_back({cell.y, cell.x});
            while (!check_queue.empty()) {
                auto& current_cell = field_[check_queue.front().first][check_queue.front().second];
                if (!current_cell.is_flag) {
                    current_cell.visibility = CellVisibilityStatus::IS_OPEN;
                    if (current_cell.status == CellStatus::IS_EMPTY) {
                        auto cycle_limits = GetRectAround(current_cell.x, current_cell.y);
                        for (size_t i = cycle_limits.y_begin; i < cycle_limits.y_end; ++i) {
                            for (size_t j = cycle_limits.x_begin; j < cycle_limits.x_end; ++j) {
                                if (!field_[i][j].is_flag && field_[i][j].visibility != CellVisibilityStatus::IS_OPEN) {
                                    check_queue.push_back({i, j});
                                }
                            }
                        }
                    }
                }
                check_queue.pop_front();
            }
        }
    }
}

void Minesweeper::NewGame(size_t width, size_t height, size_t mines_count) {
    field_.clear();
    height_ = height;
    width_ = width;
    EmptyField();
    auto bombs_to_insert = mines_count;
    auto cells_remain = height * width;
    for (size_t i = 0U; i < height_; ++i) {
        for (size_t j = 0U; j < width_; ++j) {
            if (bombs_to_insert > 0) {
                if (bombs_to_insert < cells_remain) {
                    if (rand() % 2 == 0) {
                        --bombs_to_insert;
                        field_[i][j].status = CellStatus::IS_BOMB;
                        IncNeighborsBombsCount(j, i);
                    }

                } else {
                    --bombs_to_insert;
                    field_[i][j].status = CellStatus::IS_BOMB;
                    IncNeighborsBombsCount(j, i);
                }
            }
            --cells_remain;
        }
    }
}

void Minesweeper::NewGame(size_t width, size_t height, const std::vector<Cell>& cells_with_mines) {
    field_.clear();
    game_start_time_ = 0;
    status_ = GameStatus::NOT_STARTED;
    height_ = height;
    width_ = width;
    empty_cells_count_ = height * width - cells_with_mines.size();
    EmptyField();
    for (const auto& cell : cells_with_mines) {
        field_[cell.y][cell.x].status = CellStatus::IS_BOMB;
        IncNeighborsBombsCount(cell.x, cell.y);
    }
}

time_t Minesweeper::GetGameTime() const {
    return std::time(nullptr) - game_start_time_;
}
void Minesweeper::CheckVictory() {
    size_t opened_cells_count = 0;
    for (const auto& line : field_) {
        for (const auto& cell : line) {
            opened_cells_count += (cell.visibility == CellVisibilityStatus::IS_OPEN);
        }
    }
    if (opened_cells_count == empty_cells_count_ && status_ == GameStatus::IN_PROGRESS) {
        status_ = GameStatus::VICTORY;
    }
}

Minesweeper::GameStatus Minesweeper::GetGameStatus() const {
    if (status_ != GameStatus::IN_PROGRESS) {
        return status_;
    }
    size_t opened_cells_count = 0;
    for (const auto& line : field_) {
        for (const auto& cell : line) {
            opened_cells_count += (cell.visibility == CellVisibilityStatus::IS_OPEN);
        }
    }
    if (opened_cells_count == empty_cells_count_) {
        return GameStatus::VICTORY;
    }
    return GameStatus::IN_PROGRESS;
}
