#include <iostream>
#include <vector>
#include <cstdlib>
#include <algorithm>
#include <string>
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
				cout << "- ";
			else
				cout << grid[i][j] << " ";
		}
		cout << endl;
	}
	cout << endl;
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

bool is_number(string name) // true if given string is a valid number
{
    for(int i=0; i<name.size(); i++)
        if(!isdigit(name[i]))
            return false; // if one of the characters is not a digit

    return true;
}

bool input_argument_counter(string line) //returns true if argument numbers match the "x y n" format
{
	int i=0, counter=0;
	while(i < line.size())
	{
		while(i < line.size() && line[i] == ' ')
			i++; //skip spaces

		if (i < line.size())
		{
			counter++;
			while(i < line.size() && line[i] != ' ')
			i++;
		}
	}
	
	if(counter == 3)
		return true;
	else
		return false;
}

bool extracting_arguments_from_line(string line, int& col, int& row, int& n)
{
	int i=0, counter=0, begin_range, range;
	string temp[3];

	while(i < line.size())
	{
		while(i < line.size() && line[i] == ' ')
			i++; //skip spaces

		if (i < line.size())
		{
			begin_range = i;
			while(i < line.size() && line[i] != ' ')
				i++;
			range = i - begin_range;
			temp[counter] = line.substr(begin_range, range);
			counter++;
		}
	}

	if (is_number(temp[0]) && is_number(temp[1]) && is_number(temp[2]))
	{
		col = atoi (temp[0].c_str());
		row = atoi (temp[1].c_str());
		n = atoi (temp[2].c_str());
		return true;
	}
	else
		return false;
}

bool check_valid_range (int num)  //return true if given number is in given range
{
	if (num < 1 || num > 9)
		return false;

	return true;
}

int read_request (vector<vector<int> >& grid) //returns:  0 = eof, 1 = valid request, 2 = invalid request
{
	int col,row,n;
	string line;
	if(!getline(cin, line)) //checking eof
		return 0;
	
	if(input_argument_counter(line) == false)
	{
		cout << "ERR1" << endl << endl;
		return 2;
	}

	if(extracting_arguments_from_line(line, col, row, n) == false)
	{
		cout << "ERR1" << endl << endl;
		return 2;	
	}


	if(check_valid_range(col) == false || check_valid_range(row) == false)
	{
		cout << "ERR2" << endl << endl;
		return 2;
	}

	if(check_valid_range(n) == false)
	{
		cout << "ERR3" << endl << endl;
		return 2;
	}

	//changing the grid based on the user request after validating it:
	grid[row-1][col-1] = n;
	return 1;
}


int main()
{
	vector<vector<int> > grid(9,vector<int>(9));
	int request_validation = 0;

	read_grid(grid); // initializing the grid.
	cin.ignore();

	if(check_for_empty_fields(grid))  // for checking grids that are already complete
		request_validation = read_request(grid);
	else
	{
		if(check_grid_answer(grid))
			 cout << "valid" <<endl;
	 	else
	  		cout << "invalid" <<endl;
	}

	while(request_validation)
	{
		if(check_for_empty_fields(grid))
		{
			if(request_validation == 1)	// in case request_validation==2 the grid should not be printed.
				print_grid(grid);
		}										
		else
		{
			if(check_grid_answer(grid))
				 cout << "valid" <<endl;
	 		else
	  			cout << "invalid" <<endl;
	  		break; // breaks if the grid is complete
		}

		request_validation = read_request(grid);
	}

}
