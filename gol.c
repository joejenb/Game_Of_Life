#include<stdlib.h>
#include<stdio.h>
#include"gol.h"

#define MAX_COLUMNS 512

void validate_universe(struct universe *u);

void read_in_file(FILE *infile, struct universe *u){
	int col = 0;
	int row = 0;
	int current_alive = 0;
	int next;

	if(!(u->cells = (char **) calloc(2, sizeof(char *)))){
		fprintf(stderr, "Unable to allocate necessary memory\n");
		exit(1);
	}
	if (!(u->cells[0] = (char *)calloc(2, sizeof(char)))){
		fprintf(stderr, "Unable to allocate necessary memory\n");
		exit(1);
	}

	while ((next = getc(infile))!=EOF){
		if (next == '*' || next == '.' || next == '\n'){
			if (next != '\n'){
				if (!(u->cells[row] = (char *)realloc(u->cells[row], (col+2)*sizeof(char)))){
					fprintf(stderr, "Unable to allocate necessary memory\n");
					exit(1);
				}
				if (next == '*') 
					current_alive++;

				u->cells[row][col] = (char) next;
				col++;
			}
			else{
				if (row == 0)
					u->columns = col;
				if (col != u->columns || col > MAX_COLUMNS){
					fprintf(stderr, "Invalid number of columns inputted\n");
					exit(1);
				}
				if (!(u->cells = (char **)realloc(u->cells, (row+2)*sizeof(char *)))){
					fprintf(stderr, "Unable to allocate necessary memory\n");
					exit(1);
				}
				row++;
				if(!(u->cells[row] = (char *)calloc(2, sizeof(char)))){
					fprintf(stderr, "Unable to allocate necessary memory\n");
					exit(1);
				}
				col = 0;
			}
		}
		else{
			fprintf(stderr, "Invalid symbol used\n");
			exit(1);
		}
	}

	u->rows = row;
	u->total_alive = current_alive;
	u->current_alive = current_alive;

	if (!(u->next_cells = (char **) calloc(u->rows + 1, sizeof(char *)))){
		fprintf(stderr, "Unable to allocate necessary memory\n");
		exit(1);
	}

	for (int r = 0; r < u->rows; r++){
		if (!(u->next_cells[r] = (char *)calloc(u->columns + 1, sizeof(char)))){
			fprintf(stderr, "Unable to allocate necessary memory\n");
			exit(1);
		}
	}
}

void write_out_file(FILE *outfile, struct universe *u){
	validate_universe(u);
	for (int r = 0; r < u->rows; r++){
		for (int c = 0; c < u->columns; c++){
			fprintf(outfile, "%c", u->cells[r][c]);
		}
		fprintf(outfile, "\n");
	}
}


int is_alive(struct universe *u, int column, int row){
	validate_universe(u);
	if (u->cells[row][column] == '*')
		return 1;

	return 0;
}

int will_be_alive(struct universe *u, int column, int row){
	validate_universe(u);
	int no_alive = 0;
	int am_alive = is_alive(u, column, row);

	for (int r = -1; r < 2; r++){
		for (int c = -1; c < 2; c++){
			int ne_r = row + r;
			int ne_c = column + c;
			if (ne_r >= 0 && ne_r < u->rows && ne_c >= 0 && ne_c < u->columns && (r != 0 || c != 0))
				no_alive += is_alive(u, ne_c, ne_r);
		}
	}

	if ((am_alive && (no_alive == 2)) || no_alive == 3)
		return 1;	
	
	return 0;	
}

int will_be_alive_torus(struct universe *u, int column, int row){
	validate_universe(u);
	int no_alive = 0;
	int am_alive = is_alive(u, column, row);

	for (int r = -1; r < 2; r++){
		for (int c = -1; c < 2; c++){
			int ne_r = (row + r) % u->rows;
			int ne_c = (column + c) % u->columns;
			if (r != 0 || c != 0){
				if (ne_r < 0)
					ne_r = u->rows - 1;
				if (ne_c < 0)
					ne_c = u->columns - 1;
				no_alive += is_alive(u, ne_c, ne_r);
			}
		}
	}

	if ((am_alive && (no_alive == 2)) || no_alive == 3)
		return 1;	
	
	return 0;	
}

void evolve(struct universe *u, int (*rule)(struct universe *u, int column, int row)){
	validate_universe(u);
	int current_alive = 0;
	u->gen_num++;

	for (int r = 0; r < u->rows; r++){
		for (int c = 0; c < u->columns; c++){
			if (rule(u, c, r)){
				u->next_cells[r][c] = '*';
				current_alive++;
			}
			else{
				u->next_cells[r][c] = '.';
			}
		}
	}

	u->current_alive = current_alive;
	u->total_alive += current_alive;

	for (int r = 0; r < u->rows; r++){
		for (int c = 0; c < u->columns; c++){
			u->cells[r][c] = u->next_cells[r][c]; 
		}
	}
}

void print_statistics(struct universe *u){
	validate_universe(u);
	float size = u->rows * u->columns;
	float per_calive = 100*u->current_alive/size;
	float per_talive = 100*(u->total_alive)/((u->gen_num+1) * size);

	if (!size){
		per_calive = 0;
		per_talive = 0;
	}

	printf("%.3f %% of cells currently alive\n", per_calive);
	printf("%.3f %% of cells alive on average\n", per_talive);
}	

void validate_universe(struct universe *u){
	if (!u->cells || !u->cells[0] || !u->next_cells || !u->next_cells[0]){
		printf("Input necessary to create universe structure has not been provided\n");
		exit(1);
	}
	if (u->rows < 0 || u->columns < 0 || u->columns > MAX_COLUMNS){
		printf("Invalid universe state\n");
		exit(1);
	}
}
