#include <iostream>
#include <vector>
#include <cstdlib>
#include <algorithm>
using namespace std;

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

void print_grid(vector<vector<int> > grid)
{
	for (int i = 0; i < 9; i++)
	{
		for (int j = 0; j < 9; j++)
		{
			if (grid[i][j] == -1)
				cout << "  ";
			else
				cout << grid[i][j] << " ";
		}
		cout << endl;
	}
}

bool find_equal_adjacent (vector<int> vec) // checks a vector for equal adjacent members [true if sth was found]
{
	for (int i = 0; i < vec.size()-1; i++)
		if (vec[i] == vec[i+1])
			return true;
			
	return false; //in case nothing was found
}

bool find_duplicates_in_grid_rows(vector<vector<int> > grid) // 1.sort 2.find_equal_adjacent()   [true if sth was found]
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

void test_skip_spaces()
{
	char a [30];
	for (int i = 0; i < 10 ; ++i)
	{
		skip_spaces();
		cin.get(a[i]);
	}
	for (int i = 0; i < 10; ++i)
	{
		cout << a[i] << " ";
	}
	cout << endl;
}

int main()
{
	vector<vector<int> > grid(9,vector<int>(9));
	int number_of_grids;
	cin >> number_of_grids;

	for (int i = 0; i < number_of_grids; i++)
	{
		read_grid(grid);
		//print_grid(grid);
			
		if(check_grid_answer(grid))
			cout << "true" <<endl;
		else
		 	cout << "false" <<endl;
	}
}
