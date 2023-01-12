#pragma once

#include <string>
#include <vector>

class Minesweeper {
public:
    struct Cell {
        size_t x = 0;
        size_t y = 0;
    };

    enum class GameStatus {
        NOT_STARTED,
        IN_PROGRESS,
        VICTORY,
        DEFEAT,
    };

    using RenderedField = std::vector<std::string>;

    Minesweeper(size_t width, size_t height, size_t mines_count);
    Minesweeper(size_t width, size_t height, const std::vector<Cell>& cells_with_mines);

    void NewGame(size_t width, size_t height, size_t mines_count);
    void NewGame(size_t width, size_t height, const std::vector<Cell>& cells_with_mines);

    void OpenCell(const Cell& cell);
    void MarkCell(const Cell& cell);

    GameStatus GetGameStatus() const;
    time_t GetGameTime() const;

    RenderedField RenderField() const;

private:
    struct RectAround {
        size_t y_begin;
        size_t x_begin;
        size_t y_end;
        size_t x_end;
    };

    enum class CellStatus {
        IS_BOMB,
        IS_BOMB_NEIGHBOR,
        IS_EMPTY,
    };

    enum class CellVisibilityStatus {
        IS_OPEN,
        IS_CLOSED,
    };

    struct CellWithCounters : Cell {
        CellStatus status = CellStatus::IS_EMPTY;
        CellVisibilityStatus visibility = CellVisibilityStatus::IS_CLOSED;
        bool is_flag = false;
        int bombs_count = 0;
    };

    std::vector<std::vector<CellWithCounters>> field_;
    size_t height_;
    size_t width_;
    time_t game_start_time_;
    GameStatus status_ = GameStatus::NOT_STARTED;
    size_t empty_cells_count_;

    void EmptyField();
    void IncNeighborsBombsCount(size_t x, size_t y);
    void OpenAll();
    void CheckVictory();
    RectAround GetRectAround(size_t x, size_t y);
};
