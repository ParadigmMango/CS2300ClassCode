//!  CS2300 PA_2 Linear Domination
/*!
  \file linear_domination.cc
  \author Jacob Hartt (jacobjhartt@gmail.com)
  \brief A line drawing game where the player with the most cells wins!
  \version 1.0
  \date 02-27-2023
 */

#include <eigen3/Eigen/Dense>

#include <cstdlib>
#include <fstream>
#include <iostream>
#include <string>
#include <vector>


//! Vectord2d is being typedefed as Point to make point-vector calculations simpler
typedef Eigen::Vector2d Point;

//! The barycentric coordinate which represents the midpoint of any parametric line
const double kBarycentricMidpoint = 0.5;
//! The x coordinate of a midpoint of a cell relative to the x value of the cell
const double kCellMidpointX = 0.5;
//! The y coordinate of a midpoint of a cell relative to the y value of the cell
const double kCellMidpointY = 0.5;
//! A tolerance to account for floating-point error in calculations 
const double kFloatErrorTolerance = 0.0000000000001;
//! A char which represents an empty cell.
const char kEmptyCell = '.';
//! A char which represents a black cell.
const char kBlackCell = 'X';
//! A char which represents a white cell.
const char kWhiteCell = 'O';


//!  Class containing the data and functionality of a parametric line.
/*!
  A class containing the head cell, tail cell, head point, tail point, vector,
  normalized vector and midpoint of a paramtric line.

  The class also has the ability to calculate barycentric combinations.
 */
class ParametricLine {
  public:
    //! Construct a new Parametric Line object.
    /*! 
      \param tail_cell_x the x value / row of the tail point
      \param tail_cell_y the y value / column of tail point
      \param head_cell_x the x value / row of the head point
      \param head_cell_y the y value / column of the head point
     */
    ParametricLine(const unsigned int &tail_cell_x,
                   const unsigned int &tail_cell_y,
                   const unsigned int &head_cell_x,
                   const unsigned int &head_cell_y) {
        // below, -1 accounts for the fact that the input x and ys start the
        // indeces at 1 and not 0
        head_cell_ << head_cell_x - 1,
                      head_cell_y - 1;
        tail_cell_ << tail_cell_x - 1, 
                      tail_cell_y - 1;

        head_point_ << (head_cell_x - 1.0 + kCellMidpointX),
                       (head_cell_y - 1.0 + kCellMidpointY);
        tail_point_ << (tail_cell_x - 1.0 + kCellMidpointX),
                       (tail_cell_y - 1.0 + kCellMidpointY);

        vector_ = head_point_ - tail_point_;
        vector_normalized_ = vector_.normalized();

        midpoint_ = BarycentricCombination(kBarycentricMidpoint);
    }

    //! Checks if the line is perpendicular to another line.
    /*!
      \param line the line with which perpendicularity is being determined
      \return Whether or not the lines are perpendicular
     */
    bool IsPerpendicular(ParametricLine line) const {
        return vector_.dot(line.GetVector()) == 0;
    }

    //! Gets the Head Cell object.
    /*!
      \return The Head Cell object
     */
    const Point &GetHeadCell() const {
        return head_cell_;
    }

    //! Gets the Midpoint object.
    /*!
      \return The Midpoint object
     */
    const Point &GetMidpoint() const {
        return midpoint_;
    }

    //! Gets the Tail Cell object.
    /*!
      \return The Tail Cell object
     */
    const Point &GetTailCell() const {
        return tail_cell_;
    }

    //! Gets the Vector object.
    /*! 
      \return The Vector object
     */
    const Eigen::Vector2d &GetVector() const {
        return vector_;
    }

    //! Gets the Vector Normalized object.
    /*!
      \return The Vector Normalized object
     */
    const Eigen::Vector2d &GetVectorNormalized() const {
        return vector_normalized_;
    }

  private:
    //! The vector from the tail point to the head point.
    Eigen::Vector2d vector_;
    //! The normalized form of vector_.
    Eigen::Vector2d vector_normalized_;
    //! The cell containing the tail point of the line on the board.
    Point tail_cell_;
    //! The cell containing the head point of the line on the board.
    Point head_cell_;
    //! The tail point's coordinates.
    Point tail_point_;
    //! The head point's coordinates.
    Point head_point_;
    //! The midpoint of the line's coordinates.
    Point midpoint_;

    //! Calculates and returns the barycentric combination at t along the line.
    /*!
      \param t the barycentric coordinate
      \return The barycentric combination at t
     */
    Point BarycentricCombination(const double &t) {
        return ( (1 - t) * tail_point_ ) + (t * head_point_);
    }
};


//!  Class representing a square board that contains colored cells.
/*!
  The class contains a grid which can display itself, draw lines on itself and
  count the number cells with a certain color on itself.
 */
class Board {
  public:
    //! Constructs a new Board object.
    Board() {}

    //! Constructs a new Board object.
    /*!
      \param width the width of the square board
     */
    Board(const unsigned int &width) {
        CreateGrid(width);
    }

    //! Creates the square grid stored in the board object.
    /*!
      \param width the width of the square board
     */
    void CreateGrid(const unsigned int &width) {
        // fill grid_ with empty rows
        for (int row_num = 0; row_num < width; row_num++) {
            std::vector<char> row;

            // fill each row with empty cells
            for (int col_num = 0; col_num < width; col_num++) {
                row.push_back(kEmptyCell);
            }

            grid_.push_back(row);
        }
    }

    //! Displays the contents of the board.
    /*!
      Iteratively prints the contents of the board.
     */
    void Display() {
        for (const std::vector<char> &row : grid_) {
            for (const char &cell : row) {
                std::cout << cell << ' ';
            }

            std::cout << '\n';
        }

        std::cout << '\n';
    }

    //! Draws a parametric line onto the board in a certain color.
    /*!
      The function draws an imaginary line between the midpoints of the cells
      which form the endpoints of the parametric line and colors in all cells 
      which this imaginary line touches with the input color.

      This function implements the Amanatides and Woo Algorithm which is a
      raytracing algorithm described in detail in the following GitHub document:
      https://github.com/cgyurgyik/fast-voxel-traversal-algorithm/blob/master/overview/FastVoxelTraversalOverview.md

      The main limitation of this algorithm is floating point arithmetic errors
      are cumulated over time. To address this and other issues created by 
      floating point arithmetic errors a tolerance of around 1 * 10^-12 has been
      introduced. However, the algorithm will start to produce errors if the
      line gets longer than around 1000 units.

      \pre The input line is valid / can be drawn on the board.
     */
    void PlotLine(const ParametricLine &line, const char &color) {
        // collects the objects in the line to make things more readable
        const Point &line_tail_cell = line.GetTailCell();
        const Point &line_head_cell = line.GetHeadCell();
        const Eigen::Vector2d &line_vector = line.GetVector();
        const Eigen::Vector2d &line_vector_norm = line.GetVectorNormalized();

        // colors in the head cell
        SetCell(color, line_tail_cell(0), line_tail_cell(1));

        // if the line has lenth, colors in the rest of it
        if (line_vector(0) != 0.0 || line_vector(1) != 0.0) {
            // initializes x and y
            unsigned int x = line_tail_cell(0);
            unsigned int y = line_tail_cell(1);

            // intializes x_step
            int x_step;
            if (line_vector(0) > 0.0) {
                x_step = 1;
            } else if (line_vector(0) < 0.0) {
                x_step = -1;
            } else {
                x_step = 0;
            }

            // initializes y_step
            int y_step;
            if (line_vector(1) > 0.0) {
                y_step = 1;
            } else if (line_vector(1) < 0.0) {
                y_step = -1;
            } else {
                y_step = 0;
            }

            // initializes the t_delta variables
            double t_delta_x = abs( 1 / line_vector_norm(0) );
            double t_delta_y = abs( 1 / line_vector_norm(1) );

            // initializes the t_max variables
            double t_max_x = kCellMidpointX * t_delta_x;
            double t_max_y = kCellMidpointX * t_delta_y;

            // Draws the rest of the line. This part is hard to understand unless
            // you have read the document mentioned in the comment at the head
            // of this fuction.
            do {
                if (t_max_x < t_max_y - kFloatErrorTolerance) {
                    t_max_x += t_delta_x;
                    x += x_step;
                    SetCell(color, x, y);
                } else if (t_max_x > t_max_y + kFloatErrorTolerance) {
                    t_max_y += t_delta_y;
                    y += y_step;
                    SetCell(color, x, y);
                } else {
                    t_max_x += t_delta_x;
                    t_max_y += t_delta_y;
                    x += x_step;
                    y += y_step;
                    SetCell(color, x, y);
                }
            } while (line_head_cell(0) != x || line_head_cell(1) != y);
        }
    }

    //! Writes the contents of the board and each player's score to a file.
    /*!
      \param write_file_path the file written to
     */
    void Write(const std::string &write_file_path) {
        // opens the file to write to
        std::ofstream write_file;
        write_file.open(write_file_path);

        // writes the grid data to the file
        for (std::vector<char> row : grid_) {
            for (char cell : row) {
                write_file << cell << ' ';
            }

            write_file << '\n';
        }

        // writes the player scores to the file
        write_file << "Player " << kBlackCell << ": " << CountColor(kBlackCell)
                   << " cells; Player " << kWhiteCell << ": " <<
                   CountColor(kWhiteCell) << " cells\n";

        write_file.close();
    }

    //! Gets a cell's color
    /*!
      \param row the row of the cell
      \param col the column of the cell
      \return The color / char of the cell
     */
    char GetCell(const unsigned int row, const unsigned int col) const {
        return grid_.at(row).at(col);
    }

    //! Sets a cell in the Grid object
    /*!
      \param color the color to set the Cell to
      \param row the row of the cell
      \param col the column of the cell
     */
    void SetCell(const char &color, const unsigned int &row,
                 const unsigned int &col) {
        grid_.at(row).at(col) = color;
    }

  private:
    //! The 2D array containing the colors / chars of the board in order.
    std::vector<std::vector<char>> grid_; 

    //! Counts the number of cells of a certain color in the grid.
    /*!
      \param color the color to count in the grid
      \return The number of cells of the color in the grid
     */ 
    unsigned int CountColor(const char &color) {
        unsigned int count = 0;

        for (std::vector<char> row : grid_) {
            for (char cell : row) {
                if (cell == color) {
                    count++;
                }
            }
        }

        return count;
    }
};


//! Class representing a game of Linear Domination.
/*!
  The class class most importantly contains a board and list of plays which it
  uses to simulate a game of Linear Domination with the Play() method.
 */
class Game {
  public:
    //! Constructs a new Game object.
    /*!
      \param input_file_path the path where the input file is located
      \param output_file_path the path where the output file will be located
     */
    Game(const std::string &input_file_path,
                const std::string &output_file_path) : board_() {
        output_file_path_ = output_file_path;
        
        // opens the input file
        std::ifstream input_file;
        input_file.open(input_file_path);

        // makes sure the input_file actually exists, otherwise ends the program
        assert(input_file.good());

        // gets the board width
        unsigned int board_width;
        input_file >> board_width;

        // creates the grid on the board
        board_.CreateGrid(board_width);

        // gets the previous plays checked
        input_file >> prev_plays_checked_;

        unsigned int tail_row;
        unsigned int tail_col;
        unsigned int head_row;
        unsigned int head_col;

        // iterates and collects the line data until there is no more
        while (input_file >> tail_row >> tail_col >> head_row >> head_col) {
            // creates a line from the line data and adds it to plays_
            plays_.push_back(ParametricLine(tail_row, tail_col,
                                            head_row, head_col));
        }

        input_file.close();
    }

    //! Simulates the game.
    /*!
      Simulates the game on the board using plays_.
     */
    void Play() {
        // dislays the empty state of the board.
        board_.Display();

        char curr_color;

        // iterate through plays_
        for (int play_num = 0; play_num < plays_.size(); play_num++) {
            // alternates the color of plays
            if (play_num % 2 == 0) {
                curr_color = kBlackCell;
            } else {
                curr_color = kWhiteCell;
            }

            // draws the current play if it's valid
            if (IsPlayValid(play_num)) {
                board_.PlotLine(plays_[play_num], curr_color);
            }

            // displays the new(?) board state
            board_.Display();
        }
        
        // writes the final state to the output file path
        board_.Write(output_file_path_);
    }

  private:
    //! The game's board.
    Board board_;
    //! The output path the game writes to when it finishes.
    std::string output_file_path_;
    //! The game's list of plays.
    std::vector<ParametricLine> plays_;
    //! The number of previous plays the game checks for validity.
    unsigned int prev_plays_checked_;

    //! Determines whether the play at play_num in plays_ is valid.
    /*!
      \param play_num the index of the play to determine the validity of
      \return Whether or not the play at play_num in plays_ is valid
     */
    bool IsPlayValid(unsigned int play_num) {
        // only checks if the play is valid if there are plays to check against
        if (prev_plays_checked_ > 0) {
            // stores the target play locally for convenience 
            const ParametricLine &play = plays_[play_num];

            // finds the index of the first play to check against
            int first_play_checked = play_num - prev_plays_checked_;

            if (first_play_checked < 0) {
                first_play_checked = 0;
            }

            // checks all of the necessary plays
            for (unsigned int curr_play_num = first_play_checked;
                        curr_play_num < play_num; curr_play_num++) {
                // stores the play being checked against locally for convenience
                const ParametricLine &curr_play = plays_[curr_play_num];

                // calculates the base conditions for play validity
                bool endpoints_not_empty = play.GetTailCell() ==
                                           curr_play.GetTailCell() ||
                                           play.GetHeadCell() ==
                                           curr_play.GetHeadCell();
                bool same_midpoints = play.GetMidpoint() == curr_play.GetMidpoint();
                bool plays_perpendicular = play.IsPerpendicular(curr_play);

                // if the play is invalid, returns false
                if (endpoints_not_empty || same_midpoints || plays_perpendicular) {
                    return false;
                }
            }
        }

        return true;
    }
};


//! The main function.
/*!
  \return The exit value
 */
int main() {
    // test game
    std::cout << "                        TEST GAME                        \n"
              << "---------------------------------------------------------\n";
    Game game_test = Game("pa2_input_test.txt", "pa2_output_test.txt");
    game_test.Play();
    std::cout << '\n' << '\n' << '\n' << '\n';

    // game one
    std::cout << "                          GAME 1                          \n"
              << "----------------------------------------------------------\n";
    Game game_1 = Game("pa2_input_1.txt", "pa2_output_1.txt");
    game_1.Play();
    std::cout << '\n' << '\n' << '\n' << '\n';

    // game 2
    std::cout << "                          GAME 2                          \n"
              << "----------------------------------------------------------\n";
    Game game_2 = Game("pa2_input_2.txt", "pa2_output_2.txt");
    game_2.Play();
    std::cout << '\n' << '\n' << '\n' << '\n';

    // game 3
    std::cout << "                          GAME 3                          \n"
              << "----------------------------------------------------------\n";
    Game game_3 = Game("pa2_input_3.txt", "pa2_output_3.txt");
    game_3.Play();

    return 0;
}
