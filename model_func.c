#include  <stdio.h>

#include "view_func.h"

float max(float a, float b){
	return (a > b) ? a : b;
}

float absf(float a){
	return (a > 0) ? a : -a;
}

float model_chart(int n, float salary){
	config_t cfg;
	config_init(&cfg);
	double salary_result = 0;
	if(!config_validaiton(&cfg)) return 0;
	config_setting_t *setting1, *setting2;
	setting1 = config_lookup(&cfg, "inflation_chart");
	setting2 = config_lookup(&cfg, "salary_chart");
	int size1 = config_setting_length(setting1);
	int size2 = config_setting_length(setting2);
	for(int i=0; i<n; i++){
		config_setting_t *inflation = config_setting_get_elem(setting1, (int)(i/12)%size1);
		float val_inf;
		config_setting_t *salar = config_setting_get_elem(setting2, (int)(i/12)%size2);
		float val_salar;

		val_inf = config_setting_get_float(inflation);	
		val_salar = config_setting_get_float(salar);	
			
		salary_result +=  salary * (100.f / (100.f + val_inf/12.f)) * ((100.f + val_salar/12.f) / 100.f);
	}
    return salary_result;
}

double pension_calc(int num_contr, float salary, float exp_life_time, float percent, int diff){
	return absf((num_contr*salary*percent/100 + model_chart(diff, salary)*percent/100)/(float)exp_life_time);
}

float convert_to_brutto(float netto){
	return (netto/(1.0 - 0.09 - 0.18));
	///return (netto > 556.02/12) ? (netto + (netto - 556.02/12)/(1.0 - (0.09 + 0.18))) : (netto/(1.0 - 0.09));
}

float convert_to_netto(float brutto){
	return brutto - max(0,(brutto * 0.09 + (((brutto * 0.18 - 556.02/12) >= 0) ? (brutto * 0.18 - 556.02/12) : 0)));
}

void display_result(frame* frame_list, unsigned typ, int num_contr, float salary, float exp_life_time, float percent, int diff, unsigned N){
	
	if(typ == 1){ /// Brutto
		float final_brutto = pension_calc(num_contr, salary, exp_life_time, percent, diff);
		mvwprintw(frame_list[N-1].win, 1, 1, "Obliczona wysokosc emerytury: \n\n\tBRUTTO: %.2f \n\tNETTO: %.2f", final_brutto, convert_to_netto(final_brutto));
	}
	if(typ == 0){ /// Netto
		float final_netto = pension_calc(num_contr, salary, exp_life_time, percent, diff);
		mvwprintw(frame_list[N-1].win, 1, 1, "Obliczona wysokosc emerytury: \n\n\tBRUTTO: %.2f \n\tNETTO: %.2f", convert_to_brutto(final_netto), final_netto);
	}
	
}

void result_controller(frame* frame_list, unsigned N){
	time_t timer;
	time(&timer);
	struct tm *tm_input, *tm_info;
	tm_input = (struct tm*)malloc(1);
	tm_input->tm_mon = frame_list[3].Typ->Selector->option;
	tm_input->tm_year = frame_list[4].Typ->Selector->option + 119;
						
	tm_info = localtime(&timer);
						
	int num_contr = atoi(frame_list[1].Typ->Input->val);
	float salary = atof(frame_list[2].Typ->Input->val);
	float exp_life_time = atof(frame_list[5].Typ->Input->val);
	float percent = atof(frame_list[6].Typ->Input->val);
	int diff = 12*abs((int)tm_info->tm_year - (int)tm_input->tm_year) - abs((int)tm_info->tm_mon - (int)tm_input->tm_mon);
						
	wclear(frame_list[N-1].win);
	
	display_result(frame_list, frame_list[0].Typ->Selector->option ,num_contr, salary, exp_life_time, percent, diff, N);
}


