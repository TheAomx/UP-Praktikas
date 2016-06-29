//********************************************************************
// Ton-Definitionen								Version 02.04.2012 hpw
//********************************************************************

// Ab hier: Definition der Tonhöhe
#define	f_c_1   	22934					// C
#define	f_cis_1 	21646					// CIS
#define	f_d_1   	20431					// D
#define	f_dis_1 	19285					// DIS
#define	f_e_1   	18202					// E
#define	f_f_1   	17181					// F
#define	f_fis_1 	16216					// FIS
#define	f_g_1   	15306					// G
#define	f_gis_1 	14447					// GIS
#define	f_a_1   	13636					// A
#define	f_b_1   	12871					// B
#define	f_h_1   	12149					// H
#define	f_c     	11467					// c
#define	f_cis   	10823					// cis
#define	f_d     	10216					// d
#define	f_dis   	 9642					// dis
#define	f_e     	 9101					// e
#define	f_f     	 8590					// f
#define	f_fis   	 8108					// fis
#define	f_g     	 7653					// g
#define	f_gis   	 7224					// gis
#define	f_a     	 6818					// a
#define	f_b     	 6436					// b
#define	f_h     	 6074					// h
#define	f_c1    	 5733					// c'
#define	f_cis1  	 5412					// cis'
#define	f_d1    	 5108					// d'
#define	f_dis1  	 4821					// dis'
#define	f_e1    	 4551					// e'
#define	f_f1    	 4295					// f'
#define	f_fis1  	 4054					// fis'
#define	f_g1    	 3827					// g'
#define	f_gis1  	 3612					// gis'
#define	f_a1    	 3409					// a'
#define	f_b1    	 3218					// b'
#define	f_h1    	 3037					// h'
#define	f_c2    	 2867					// c''
#define	f_cis2  	 2706					// cis''
#define	f_d2    	 2554					// d''
#define	f_dis2  	 2411					// dis''
#define	f_e2    	 2275					// e''
#define	f_f2    	 2148					// f''
#define	f_fis2  	 2027					// fis''
#define	f_g2    	 1913					// g''
#define	f_gis2  	 1806					// gis''
#define	f_a2    	 1705					// a''
#define	f_b2    	 1609					// b''
#define	f_h2    	 1519					// h''
#define	f_c3    	 1433					// c'''
#define	f_cis3  	 1353					// cis'''
#define	f_d3    	 1277					// d'''
#define	f_dis3  	 1205					// dis'''
#define	f_e3    	 1138					// e'''
#define	f_f3    	 1074					// f'''
#define	f_fis3  	 1014					// fis'''
#define	f_g3    	  957					// g'''
#define	f_gis3  	  903					// gis'''
#define	f_a3    	  852					// a'''
#define	f_b3    	  804					// b'''
#define	f_h3    	  759					// h'''
#define	f_c4    	  717					// c''''
#define	f_pause 	    0					// keine Tonausgabe

// Ab hier: Definition der Tondauer
#define	Timer_ISR	1                   	// Dauer zwischen zwei ISR-Aufrufen [ms]
#define	ViertelNote	(60000/BPM)         	// Dauer einer 1/4 Note [ms]

#define	d_n4    	(ViertelNote/Timer_ISR)	// 1/4 Note
#define	d_n64   	d_n4/16             	// 1/64 Note
#define	d_n32   	d_n4/8              	// 1/32 Note
#define	d_n16   	d_n4/4              	// 1/16 Note
#define	d_n16p  	d_n16+d_n32         	// punktierte 1/16 Note
#define	d_n8    	d_n4/2              	// 1/8 Note
#define	d_n8p   	d_n8+d_n16          	// punktierte 1/8 Note
#define	d_n4p   	d_n4+d_n8           	// punktierte 1/4 Note
#define	d_n2    	d_n4*2              	// 1/2 Note
#define	d_n2p   	d_n2+d_n4           	// punktierte 1/2 Note
#define	d_n1    	d_n4*4              	// ganze Note
#define	d_ENDE  	0                   	// Ende des Musikstückes
