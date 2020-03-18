#include <iostream>
#include <vector>
#include <cstdlib>
#include <algorithm>
#include <string>
#include "rsdl.h"
using namespace std;

#define SIZE 30
#define Width 270
#define Height 270
#define ROW 9
#define COL 9
/*
	void draw_point(int x, int y, RGB color);
	void draw_line(int x1, int y1, int x2, int y2, RGB color);
	void draw_rect(int x, int y, int w, int h, RGB color);
	void fill_rect(int x, int y, int w, int h, RGB color);
	void draw_bmp(string filename, int x, int y);
	void draw_circle(int x, int y, int r, RGB color);
	void fill_circle(int x, int y, int r, RGB color);
*/
void print_grid(vector<vector<int> > grid)
{
	for (int i = 0; i < 9; i++)
	{
		for (int j = 0; j < 9; j++)
		{
			if (grid[i][j] == -1)
				cout << "- ";
			else
				cout << grid[i][j] << " ";
		}
		cout << endl;
	}
}

void skip_spaces() //will skip all of the spaces before an actual character
{
	char temp;
	cin >> temp;
	while (temp == ' ')
		cin >> temp;
	cin.unget();  //putting the wrongly read char back to the input stream
}

void read_grid(vector<vector<int> >& grid)
{
	char temp;
	for (int i = 0; i < 9; i++)
		for (int j = 0; j < 9; j++)
		{
			skip_spaces();
			cin >> temp;
			if (temp == '#')
				grid[i][j] = -1; // -1 represents an empty field
			else
				grid[i][j] = atoi(&temp);
		}
}

bool check_for_empty_fields(vector<vector<int> > grid) //check if all of the fields are filled [true if sth was found]
{
	for (int i = 0; i < 9; i++)
		for (int j = 0; j < 9; j++)
		{
			if (grid [i][j] == -1)
				return true;
		}

	return false; // if no empty field was found
}

bool find_equal_adjacent (vector<int> vec) // checks a vector for equal adjacent members [true if sth was found]
{
	for (int i = 0; i < vec.size()-1; i++)
		if (vec[i] == vec[i+1] && vec[i] != -1)
			return true;
			
	return false; //in case nothing was found
}

bool find_duplicates_in_grid_rows(vector<vector<int> > grid) // 1.sort 2.find_equal_adjacent()  [true if sth was found]
{
	for (int i = 0; i < 9; ++i)
	{
		sort(grid[i].begin(), grid[i].end());
		if (find_equal_adjacent(grid[i]))
			return true; // in case of finding a duplicate
	}

	return false; // in case of no duplicates
}

bool find_duplicates_in_grid_columns(vector<vector<int> > grid) // [true if sth was found]
{
	// 1.reversing the grid:
	vector<vector<int> > reversed_grid(9,vector<int>(9));
	for (int i = 0; i < 9; i++)
		for (int j = 0; j < 9; j++)
			reversed_grid[j][i] = grid[i][j];

	// 2.calling find_duplicates_in_grid_rows
	return find_duplicates_in_grid_rows(reversed_grid);
}

bool find_duplicates_in_sub_grids (vector<vector<int> > grid) // [true if sth was found]
{
	vector<int> sub_grid (9);
	int i_base = -1;
	int j_base = 0;
	int delta_i[9] = {0,0,0,1,1,1,2,2,2};
	int delta_j[9] = {0,1,2,0,1,2,0,1,2};
	
	for (int i = 0; i < 9; ++i)
	{
		j_base = i % 3;
		if(i % 3 == 0)
			i_base++;

		for (int j = 0; j < 9; ++j) // extracting sub_grid
			sub_grid[j] = grid [3*i_base + delta_i[j]] [3*j_base + delta_j[j]];

		sort (sub_grid.begin(), sub_grid.end());// sorting the extracted sub_grid
		if(find_equal_adjacent(sub_grid))
			return true; //on finding duplicates
	}

	return false; // in case of finding nothing
}

bool find_duplicates_in_grid (vector<vector<int> > grid) // first sorts then calls find_equal_adjacent (first for rows, then columns) [true if sth was found]
{
	if(find_duplicates_in_grid_rows(grid))
		return true;
	if(find_duplicates_in_grid_columns(grid))
		return true;
	if(find_duplicates_in_sub_grids(grid))
		return true;

	return false; // in case of finding no duplicates at all
}

bool check_grid_answer(vector<vector<int> > grid)
{
	if (check_for_empty_fields(grid))
		return false; // answer is wrong due to having empty fields

	if (find_duplicates_in_grid(grid))
		return false; // answer is wrong due to duplication of fields

	return true; // if the answer is correct
}	

//////////////////////////////////////////////////////////////////////////////////////// --- GUI Functions:
void draw_grid(window& w)
{
	for (int i = 1; i < ROW; ++i)
		w.draw_line(0, i*SIZE, Width, i*SIZE, BLACK);
	for (int i = 1; i < COL; ++i)
		w.draw_line(i*SIZE, 0, i*SIZE, Height, BLACK);	
}

void highlight_field(window& w, int current_x_possition, int current_y_possition)
{

	w.fill_rect(current_x_possition * SIZE +1, current_y_possition * SIZE +1, 9, SIZE-1, GREEN);
}

void undo_highlight(window& w, int current_x_possition, int current_y_possition)
{
	w.fill_rect(current_x_possition * SIZE +1, current_y_possition * SIZE +1, 9, SIZE-1, WHITE);
}

void update_current_possition(window& w, string key, int& current_x_possition, int& current_y_possition)
{
	if(key == "UP")
	{
		if(current_y_possition == 0)
			return ;

		undo_highlight(w, current_x_possition, current_y_possition);
		current_y_possition--;
		highlight_field(w, current_x_possition, current_y_possition);
	}
	if(key == "DOWN")
	{
		if(current_y_possition == 8)
			return;

		undo_highlight(w, current_x_possition, current_y_possition);
		current_y_possition++;
		highlight_field(w, current_x_possition, current_y_possition);
	}
	if(key == "RIGHT")
	{
		if(current_x_possition == 8)
			return;

		undo_highlight(w, current_x_possition, current_y_possition);
		current_x_possition++;
		highlight_field(w, current_x_possition, current_y_possition);
	}
	if(key == "LEFT")
	{
		if(current_x_possition == 0)
			return;

		undo_highlight(w, current_x_possition, current_y_possition);
		current_x_possition--;
		highlight_field(w, current_x_possition, current_y_possition);
	}
}

void print_number(window& w, int number, int current_x_possition, int current_y_possition)
{	
	if(number == -1)
		return;
	if(number == 1)
		w.draw_bmp("./Input_Pictures/one.bmp",current_x_possition * SIZE +10, current_y_possition * SIZE +1);
	if(number == 2)
		w.draw_bmp("./Input_Pictures/two.bmp",current_x_possition * SIZE +10, current_y_possition * SIZE +1);
	if(number == 3)
		w.draw_bmp("./Input_Pictures/three.bmp",current_x_possition * SIZE +10, current_y_possition * SIZE +1);
	if(number == 4)
		w.draw_bmp("./Input_Pictures/four.bmp",current_x_possition * SIZE +10, current_y_possition * SIZE +1);
	if(number == 5)
		w.draw_bmp("./Input_Pictures/five.bmp",current_x_possition * SIZE +10, current_y_possition * SIZE +1);
	if(number == 6)
		w.draw_bmp("./Input_Pictures/six.bmp",current_x_possition * SIZE +10, current_y_possition * SIZE  +1);
	if(number == 7)
		w.draw_bmp("./Input_Pictures/seven.bmp",current_x_possition * SIZE +10, current_y_possition * SIZE +1);
	if(number == 8)
		w.draw_bmp("./Input_Pictures/eight.bmp",current_x_possition * SIZE +10, current_y_possition * SIZE +1);
	if(number == 9)
		w.draw_bmp("./Input_Pictures/nine.bmp",current_x_possition * SIZE +10, current_y_possition * SIZE +1);
}

void print_grid_numbers(window& w, vector<vector<int> > grid)
{
	for (int i = 0; i < ROW; ++i)
		for (int j = 0; j < COL; ++j)
			print_number(w, grid[j][i], i, j);
}

///////////////////////////////////////////////////////////////////////////////// -- Visual Aid Functions:
void aid_find_target_subgrid(int& sub_x, int& sub_y, int current_x_possition, int current_y_possition)
{
	if(current_x_possition < 3)
	{
		if(current_y_possition < 3)
		{
			sub_x = 0;
			sub_y = 0;
		}	
		else if(current_y_possition < 6)
		{
			sub_x = 0;
			sub_y = 3;
		}
		else if (current_y_possition < 9)
		{
			sub_x = 0;
			sub_y = 6;
		}
	}
	else if(current_x_possition < 6)
	{
		if(current_y_possition < 3)
		{
			sub_x = 3;
			sub_y = 0;
		}	
		else if(current_y_possition < 6)
		{
			sub_x = 3;
			sub_y = 3;
		}
		else if (current_y_possition < 9)
		{
			sub_x = 3;
			sub_y = 6;
		}
	}
	else if(current_x_possition < 9)
	{
		if(current_y_possition < 3)
		{
			sub_x = 6;
			sub_y = 0;
		}	
		else if(current_y_possition < 6)
		{
			sub_x = 6;
			sub_y = 3;
		}
		else if (current_y_possition < 9)
		{
			sub_x = 6;
			sub_y = 6;
		}
	}
}

void undo_row_error(window& w, int current_x_possition, int current_y_possition)
{
	int row_number = current_y_possition ;

	for (int i = 0; i < ROW; ++i)
		w.fill_rect(i * SIZE +1, row_number * SIZE +1, 9, SIZE-1, WHITE);

	highlight_field(w, current_x_possition, current_y_possition);
}

void undo_col_error(window& w, int current_x_possition, int current_y_possition)
{
	int col_number = current_x_possition ;

	for (int i = 0; i < COL; ++i)
		w.fill_rect(col_number * SIZE +1, i * SIZE +1, 9, SIZE-1, WHITE);

	highlight_field(w, current_x_possition, current_y_possition);
}

void undo_subgrid_error(window& w, int sub_x, int sub_y, int current_x_possition, int current_y_possition)
{
	for (int i = 0; i < 3; ++i)
		for (int j = 0; j < 3; ++j)
			w.fill_rect((sub_x*SIZE) + (i*SIZE) +1, (sub_y*SIZE) + (j*SIZE) +1, 9, SIZE-1, WHITE);

	highlight_field(w, current_x_possition, current_y_possition);
}

void show_row_error(window& w, int current_x_possition, int current_y_possition)
{
	int row_number = current_y_possition ;

	for (int i = 0; i < ROW; ++i)
		w.fill_rect(i * SIZE +1, row_number * SIZE +1, 9, SIZE-1, RED);

	SDL_Delay(180);
	undo_row_error(w, current_x_possition, current_y_possition);
}

void show_col_error(window& w, int current_x_possition, int current_y_possition)
{
	int col_number = current_x_possition ;

	for (int i = 0; i < COL; ++i)
		w.fill_rect(col_number * SIZE +1, i * SIZE +1, 9, SIZE-1, RED);

	SDL_Delay(180);
	undo_col_error(w, current_x_possition, current_y_possition);
}

void show_subgrid_error(window& w, int current_x_possition, int current_y_possition)
{
	int sub_x, sub_y;
	aid_find_target_subgrid(sub_x, sub_y, current_x_possition, current_y_possition);

	for (int i = 0; i < 3; ++i)
		for (int j = 0; j < 3; ++j)
			w.fill_rect((sub_x*SIZE) + (i*SIZE) +1, (sub_y*SIZE) + (j*SIZE) +1, 9, SIZE-1, RED);

	SDL_Delay(180);
	undo_subgrid_error(w, sub_x, sub_y, current_x_possition, current_y_possition);
}

string aid_error_checking(window& w, int current_x_possition, int current_y_possition, int number, vector<vector<int> > grid)
{
	grid[current_y_possition][current_x_possition] = number;
	
	if(find_duplicates_in_grid_rows(grid))
	{
		show_row_error(w, current_x_possition, current_y_possition);
		return "error";
	}
	if(find_duplicates_in_grid_columns(grid))
	{
		show_col_error(w, current_x_possition, current_y_possition);
		return "error";
	}
	if(find_duplicates_in_sub_grids(grid))
	{
		show_subgrid_error(w, current_x_possition, current_y_possition);
		return "error";
	}

	return "no_error";
}

int main(int argc, char* argv[])
{
	vector<vector<int> > grid(9,vector<int>(9));
	int current_x_possition = 0, current_y_possition = 0;
	bool quit = false, aid = false;
	string aid_error;

	if(argc > 1)
		if((string) argv[1] == "-aid")
			aid = true;

	read_grid(grid); // reading the grid from stdcin.

	window w(Width,Height);
	draw_grid(w);
	print_grid_numbers(w, grid);
	highlight_field(w, current_x_possition, current_y_possition);	

	while(!quit)
	{
		//Check for end of the game:
			if(!check_for_empty_fields(grid))
			{
				if(check_grid_answer(grid))
				{
					print_grid(grid);
					cout << "valid" <<endl;
					w.draw_bmp("./Input_Pictures/valid.bmp", 0 ,0);
				}
				else
				{	print_grid(grid);
					cout<< "invalid" <<endl;
					w.draw_bmp("./Input_Pictures/invalid.bmp", 0 ,0);
				}
			quit = true;
			break;
			}

		HANDLE
		(
			QUIT(quit = true)
			KEY_PRESS(p,
			{
				print_grid(grid);
			})

			//Arrow Keys:
			KEY_PRESS(UP,
			{
				update_current_possition(w, "UP", current_x_possition, current_y_possition);
			})
			KEY_PRESS(DOWN,
			{
				update_current_possition(w, "DOWN", current_x_possition, current_y_possition);
			})
			KEY_PRESS(RIGHT,
			{
				update_current_possition(w, "RIGHT", current_x_possition, current_y_possition);
			})
			KEY_PRESS(LEFT,
			{
				update_current_possition(w,"LEFT", current_x_possition, current_y_possition);
			})

			//Numbers:
			KEY_PRESS(1,
			{
				if(aid)
				{
					aid_error = aid_error_checking(w, current_x_possition, current_y_possition, 1, grid);
					if(aid_error == "no_error")
					{
						grid[current_y_possition][current_x_possition] = 1; // making changes to the actual grid 
						print_number(w, 1, current_x_possition, current_y_possition); //making changes to the GUI window
					}
				}
				else
				{
					grid[current_y_possition][current_x_possition] = 1; // making changes to the actual grid 
					print_number(w, 1, current_x_possition, current_y_possition); //making changes to the GUI window
				}
			})

			KEY_PRESS(2,
			{
				if(aid)
				{
					aid_error = aid_error_checking(w, current_x_possition, current_y_possition, 2, grid);
					if(aid_error == "no_error")
					{
						grid[current_y_possition][current_x_possition] = 2; // making changes to the actual grid 
						print_number(w, 2, current_x_possition, current_y_possition); //making changes to the GUI window
					}
				}
				else
				{
					grid[current_y_possition][current_x_possition] = 2; // making changes to the actual grid 
					print_number(w, 2, current_x_possition, current_y_possition); //making changes to the GUI window
				}
			})
			KEY_PRESS(3,
			{
				if(aid)
				{
					aid_error = aid_error_checking(w, current_x_possition, current_y_possition, 3, grid);
					if(aid_error == "no_error")
					{
						grid[current_y_possition][current_x_possition] = 3; // making changes to the actual grid 
						print_number(w, 3, current_x_possition, current_y_possition); //making changes to the GUI window
					}
				}
				else
				{
					grid[current_y_possition][current_x_possition] = 3; // making changes to the actual grid 
					print_number(w, 3, current_x_possition, current_y_possition); //making changes to the GUI window
				}
			})
			KEY_PRESS(4,
			{
				if(aid)
				{
					aid_error = aid_error_checking(w, current_x_possition, current_y_possition, 4, grid);
					if(aid_error == "no_error")
					{
						grid[current_y_possition][current_x_possition] = 4; // making changes to the actual grid 
						print_number(w, 4, current_x_possition, current_y_possition); //making changes to the GUI window
					}
				}
				else
				{
					grid[current_y_possition][current_x_possition] = 4; // making changes to the actual grid 
					print_number(w, 4, current_x_possition, current_y_possition); //making changes to the GUI window
				}			
			})
			KEY_PRESS(5,
			{
				if(aid)
				{
					aid_error = aid_error_checking(w, current_x_possition, current_y_possition, 5, grid);
					if(aid_error == "no_error")
					{
						grid[current_y_possition][current_x_possition] = 5; // making changes to the actual grid 
						print_number(w, 5, current_x_possition, current_y_possition); //making changes to the GUI window
					}
				}
				else
				{
					grid[current_y_possition][current_x_possition] = 5; // making changes to the actual grid 
					print_number(w, 5, current_x_possition, current_y_possition); //making changes to the GUI window
				}
			})
			KEY_PRESS(6,
			{
				if(aid)
				{
					aid_error = aid_error_checking(w, current_x_possition, current_y_possition, 6, grid);
					if(aid_error == "no_error")
					{
						grid[current_y_possition][current_x_possition] = 6; // making changes to the actual grid 
						print_number(w, 6, current_x_possition, current_y_possition); //making changes to the GUI window
					}
				}
				else
				{
					grid[current_y_possition][current_x_possition] = 6; // making changes to the actual grid 
					print_number(w, 6, current_x_possition, current_y_possition); //making changes to the GUI window
				}
			})
			KEY_PRESS(7,
			{
				if(aid)
				{
					aid_error = aid_error_checking(w, current_x_possition, current_y_possition, 7, grid);
					if(aid_error == "no_error")
					{
						grid[current_y_possition][current_x_possition] = 7; // making changes to the actual grid 
						print_number(w, 7, current_x_possition, current_y_possition); //making changes to the GUI window
					}
				}
				else
				{
					grid[current_y_possition][current_x_possition] = 7; // making changes to the actual grid 
					print_number(w, 7, current_x_possition, current_y_possition); //making changes to the GUI window
				}
			})
			KEY_PRESS(8,
			{
				if(aid)
				{
					aid_error = aid_error_checking(w, current_x_possition, current_y_possition, 8, grid);
					if(aid_error == "no_error")
					{
						grid[current_y_possition][current_x_possition] = 8; // making changes to the actual grid 
						print_number(w, 8, current_x_possition, current_y_possition); //making changes to the GUI window
					}
				}
				else
				{
					grid[current_y_possition][current_x_possition] = 8; // making changes to the actual grid 
					print_number(w, 8, current_x_possition, current_y_possition); //making changes to the GUI window
				}
			})
			KEY_PRESS(9,
			{
				if(aid)
				{
					aid_error = aid_error_checking(w, current_x_possition, current_y_possition, 9, grid);
					if(aid_error == "no_error")
					{
						grid[current_y_possition][current_x_possition] = 9; // making changes to the actual grid 
						print_number(w, 9, current_x_possition, current_y_possition); //making changes to the GUI window
					}
				}
				else
				{
					grid[current_y_possition][current_x_possition] = 9; // making changes to the actual grid 
					print_number(w, 9, current_x_possition, current_y_possition); //making changes to the GUI window
				}
			})
		)
	}

	quit = false;
	while(!quit)
	{
		HANDLE
			(
				QUIT(quit = true)
				KEY_PRESS(q,quit = true;)
			)
	}
	
	return 0;
}