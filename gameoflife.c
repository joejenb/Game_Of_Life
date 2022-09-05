#include<string.h>
#include<ctype.h>
#include<stdio.h>
#include<stdlib.h>
#include"gol.h"

int check_number(char num[]);
void clean_up(FILE *infile, FILE *outfile);

int main(int argc, char *argv[]){
	struct universe *v = malloc(sizeof(struct universe));

	FILE *infile, *outfile;
	infile = stdin;
	outfile = stdout;

	int (*rule)(struct universe *u, int column, int row);
	int stats = 0;
	rule = will_be_alive;

	char *infile_name = NULL;
	char *outfile_name = NULL;
	int gen_num_def = 0;
	int gen_num = 5;

	for(int clopt=1; clopt < argc; clopt++){
		if (argv[clopt][0] == '-'){
			if (2 == strlen(argv[clopt])){
				char tag = argv[clopt][1];
				char *spec = argv[clopt+1];
				switch (tag){
					case 'i':
						if (infile_name != NULL && strcmp(spec, infile_name)){
							clean_up(infile, outfile);
							fprintf(stderr, "Conflicting input filenames have been entered\n");
							exit(1);
						}
						if ((infile = fopen(spec, "r")) == NULL){
							clean_up(infile, outfile);
							fprintf(stderr, "Invalid input filename entered\n");
							exit(1);
						}
						infile_name = (char *) calloc(strlen(spec)+1, sizeof(char));
						infile_name = spec;
						break;
					case 'o':
						if (outfile_name != NULL && strcmp(spec, outfile_name)){
							clean_up(infile, outfile);
							fprintf(stderr, "Conflicting output filenames have been entered\n");
							exit(1);
						}
						if ((outfile = fopen(spec, "w")) == NULL ){
							clean_up(infile, outfile);
							fprintf(stderr, "Invalid output filename entered\n");
							exit(1);
						}
						outfile_name = (char *) calloc(strlen(spec)+1, sizeof(char));
						outfile_name = spec;
						break;
					case 'g':
						if (!check_number(spec)){
							clean_up(infile, outfile);
							fprintf(stderr, "Generation number must be an integer value\n");
							exit(1);
						}
						if (gen_num_def == gen_num && gen_num_def != atoi(spec)){
							clean_up(infile, outfile);
							fprintf(stderr, "Conflicting generation numbers have been entered\n");
							exit(1);
						}
						gen_num_def = atoi(spec);
						gen_num = gen_num_def;
						break;
					case 's':
						stats = 1;
						break;
					case 't':
						if ((rule = will_be_alive_torus) == NULL){
							clean_up(infile, outfile);
							fprintf(stderr, "Error assigning function pointer: 			will_be_alive_torus\n");
							exit(1);
						}
						break;
					default:
						clean_up(infile, outfile);
						fprintf(stderr, "Invalid flag entered\n");
						exit(1);
				}
			} else{
				clean_up(infile, outfile);
				fprintf(stderr, "Invalid flag format used\n");
				exit(1);
			}
		}
	}
	read_in_file(infile, v);

	for (int g = 0; g < gen_num; g++){
		evolve(v, rule);
	}
	write_out_file(outfile, v);
	
	if (stats)
		print_statistics(v);	

	return 0;
}

int check_number(char num[]){
	if (num[0] == '-')
		return 0;

	for (int i = 0; num[i] != 0; i++){
		if (!isdigit(num[i]))
			return 0;
	}
	return 1;
}

void clean_up(FILE *infile, FILE *outfile){
	if (infile != stdin && infile != NULL)
		fclose(infile);

	if (outfile != stdout && outfile != NULL)
		fclose(outfile);
}
