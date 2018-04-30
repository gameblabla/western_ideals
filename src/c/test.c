/*

The MIT License (MIT)

Copyright (c) 2018 Gameblabla

Permission is hereby granted, free of charge, to any person obtaining a copy of this software and associated documentation files (the "Software"), 
to deal in the Software without restriction, 
including without limitation the rights to use, copy, modify, merge, publish, distribute, sublicense, and/or sell copies of the Software, 
and to permit persons to whom the Software is furnished to do so, subject to the following conditions:

*/
#include <pebble.h>

static Window *my_window;
static TextLayer *text_layer;
static Layer *myCanvas;

static GBitmap *current_background;
static GBitmap *bmp[6];

static unsigned char game_mode = 0;
static unsigned char remember_back;
static unsigned char buttons[3];
static unsigned char wait_press = 0;
static unsigned char gameover_c = 0;

/* Progress[0] : Overall ingame progression
 * Progress[1] : See insersection[53]
*/
static unsigned char progress[2];
static unsigned char about_to_go;
static unsigned char titlescreen_press_start_time;
static unsigned char time_tit ;
static short titlescreen_y;

static unsigned char twice;
static unsigned char choice;

static unsigned char x_man;
static unsigned char x_enn[7];
static short y_enn[7];
static unsigned char state_enn[7], anim_enn[7];
static short enemies_number;
static unsigned char shooting;

static short rand_a_b(short a, short b)
{
    return rand()%(b-a) +a;
}

/* Action button */
static void sel_click_handler(ClickRecognizerRef recognizer, void *context) {buttons[0] = 1;}
static void sel_release_handler(ClickRecognizerRef recognizer, void *context) {buttons[0] = 0;}
/* Up button */
static void sel_click_handler_up(ClickRecognizerRef recognizer, void *context) {buttons[1] = 1;}
static void sel_release_handler_up(ClickRecognizerRef recognizer, void *context) {buttons[1] = 0;}
/* Down button */
static void sel_click_handler_down(ClickRecognizerRef recognizer, void *context) {buttons[2] = 1;}
static void sel_release_handler_down(ClickRecognizerRef recognizer, void *context) {buttons[2] = 0;}

const char text
[140] /* Number of dialogues */
[52] /* Size of text */
= { 
	{"It all started when you met a cute boi."},
	{"This really beautiful young man is Bakaka."},
	{"He's totally not gay, i swear man."},
	{"And obviously you're not horny for him."},
	{"Just pretend you're just friends, okay ?"},
	{"Let's go. And don't push him too far."},
	{"Bakaka: Hey,\n how are you doing ?"},	//8
	{""},
	{"Bakaka: ... You can't be serious. Right?"},
	{""},
	{"Bakaka: How dare you? I don't deserve this."},
	{"Bakaka: Just... DIE !"},
	{"g"},
	{"Bakaka: I'll pretend nothing happened..."},	// 13
	{"Bakaka: Awesome . Let's go somewhere !"},	// 14
	{"Where should we go ?"},	// 15
	{"1"},
	{"2"},
	{"3"},
	{"4"},
	{"Bakaka: Alright, let's head\nfor the US-Mex Border."},
	{"Location : US-Mexico Border"},
	{"Bakaka: We're here !\nStrange place to go huh?"},
	{"Bakaka: It since became an attraction park for"},
	{"tourists.You can even shoot at incoming migrants !"},
	{"? : Excuse me."},
	{"Jonas : My name is Alexander Jonas."},
	{"Why does this white-haired dude look like a"},
	{"faggot? Don't even think of having sex with my"},
	{"car, is that clear ? I know you plot something!"},
	{"Bakaka: Can you..ugh... nevermind."},
	{"1OO years ago, we were feeding black babies to"},
	{"the alligators.But those babies were communists!"},
	{"Now that we've purged the US of Democrats, it's"},
	{"time to get rid of those dirty mexicans !"},
	{"School shootings, Tornadoes, Famines...They"},
	{"were behind them all! They want to destroy"},
	{"MY country and our right to bear arms !"},
	{"Let us cleanse the world of our oppressors."},
	{"...Jonas goes away."},
	{"Bakaka: Gee, does this guy even have a life ?"},
	{"He would choke on a piece of meat while he's"},
	{"thinking about his next conspiracy plot..."},
	{"Anyway, let's go to the shooting gallery."},
	{"'Shooting Gallery'"},
	{"Here we are! But i don't see any targets..."},
	{"Dude: Damnit, where are those miggers ?"},
	{"Dude: I see one ! Shoot at it !"},
	{"*Gun fire*"},
	{"Dude: Yeah, target down! That'll teach em!"},
	{"Mate i tell you, it's better than fishing."},
	{"Bakaka: I tought those were Crisis actors..."},
	{"I feel like i want to vomit..."},
	{"I'll be right back, just stand still."},
	{"Dude: I see an army of miggers ! Help !"},
	{"Hey you over there! Grab a gun and help me."},
	{""},
	{"You: I'm not interested to leave a mess."},
	{"Dude: Are you a democrat? You sound like one!"},
	{"Dude: Well thank you, moron !"},
	{"10 minutes later."},
	{"Bakaka: Sorry, i'm back. What happened?"},
	{"You: Some guy left a mess outside."},
	{"Bakaka: Great, this is more fucked up than i"},
	{"expected it would be. Let's go home."},
	{"A few hours later, we're home."},
	{"^"},
	{"Dude: Then grab that rifle and help me !"},
	{"Let's rock and roll !"},
	{"V"},	// Mini-game
	{"You : And that should be it !"},
	{"... Fuck. They're bleeding for real."},
	{"I guess they really weren't crisis actors."},
	{"Bakaka: Sorry, i'm back. What happened?"},
	{"Bakaka: ... Welp. That's a bloody mess."},
	{"Bakaka: Did you do this ?"},
	{"Dude: Nah, it was me.Thanks to my trusty rifle!"},
	{"Bakaka: You murderer... Let's get out of here."},
	{"A few hours later, we're home."},
	{"^"},
	{"Alright, let's go to Syria !"},
	{"Location: Somewhere in Syria."},
	{"*Rumbles*"},
	{"Bakaka: What's going on? I thought it was over !"},
	{"*Rumbles*"},
	{"Bakaka: Guess not..."},
	{"Bakaka: This is fucked up. We should get out!"},
	{""},
	{"Bakaka: Are you crazy?! We gonna die!"},
	{""},
	{"Bakaka: Why do you insist so much ?!"},
	{""},
	{"Bakaka: A kiss ? Here? You're kidding !"},
	{"Bakaka: Something's coming our way..."},
	{"g"},
	{"Bakaka: Hurry! Let's go to the border instead."},
	{"We decided to go to the US-Mex border instead."},
	{"Let's hope it's not as chaotic..."},
	{"i"},
	{"Bakaka: Pfiu! That was... interesting."},
	{"Bakaka: What did you think of it?"},
	{""},
	{"Bakaka: Let's say that i like to live"},
	{"Bakaka: dangerously !"},
	{"Bakaka: Anything else to say ?"},
	{""},
	{"Bakaka: Haha, thanks ! We almost died though !"},
	{"Bakaka: Anything else to say ?"},
	{""},
	{"Bakaka: ... Why are you telling me this ?"},
	{"Bakaka: You... traitor."},
	{"Bakaka: Do you realise what this means?"},
	{"You: You almost got myself killed !"},
	{"You: Frankly, you're an idiot."},
	{"Bakaka: ...Why. After all we've done."},
	{"Bakaka: Fine, whatever. As you wish."},
	{"Bakaka: I'll make sure to end this quickly."},
	{"*Slash*"},
	{"2"}, // Bad ending activated
	{"Bakaka: ...Well, interesting."},
	{"Bakaka: I could give you a try i suppose."},
	{"Bakaka: Hey, what are you doing ?"},
	{"Bakaka: ..."},
	{"Bakaka: Nice dick."},
	{"Bakaka: Nice shape too !"},
	{"Bakaka: I'll suck it for sure !"},
	{"Bakaka: ...\nin a future sequel."},
	{"And then they lived happily after. TEH END"},
	{"Sucks huh ? No sex scene !"},
	{"Just be happy that you got to see a dick, ok?"},
	{"Oh and this is the last Pebble game too."},
	{"So don't complain, mokay ?"},
	{"I had to rush that shit."},
	{"See you later."},
	{"Made by Gameblabla"},
	{"https://gameblabla.nl"},
	{"q"}
};


const unsigned char face[140] =
{ 
	0,
	0, 
	0,
	0,
	0,
	0,
	1,// Fine, Get out faggot (Choice)
	1,// If you chose faggot, tells you "You can't be serious"
	4,
	4,
	4,
	0,
	0,
	3,
	1,
	1,
	8,
	8,
	8,
	8,
	8,
	9,
	11,
	11,
	11,
	11,
	10,
	10,
	10,
	10,
	11,
	10,
	10,
	10,
	10,
	10,
	10,
	10,
	10,
	9,
	11,
	11,
	11,
	11,
	12,
	16,
	12,
	13,
	14,
	15,
	15,
	17,
	17,
	17,
	15,
	15,
	15,
	15,
	15,
	15,
	0,
	11,
	11,
	11,
	11,
	8,
	0,
	15,
	15,
	15,
	20,
	20,
	20,
	20,
	21,
	21,
	21,
	20,
	21,
	8,
	8,
	22,
	22,
	23,
	22,
	23,
	23,
	23,
	23,
	23,
	23,
	23,
	23,
	23,
	24,
	23,
	8,
	8,
	0,
	2,
	1,//What did you think of it
	1, 
	3,
	3,
	1,
	1,
	2,
	1,
	1,
	4,
	4,
	4,
	4,
	4,
	4,
	1,
	1,
	0,
	0,
	3,
	1,
	1,
	25,
	26,
	27,
	27,
	27,
	0,
	0,
	0,
	0,
	0,
	0,
	0,
	0,
	0,
	0,
	0,
	0,
	0
};


const char choice_text
[20] /* Number of dialogues */
[50] /* Size of text */
= { 
	{"    Fine\n    Get out faggot"},
	{"    I am, faggot!\n   Just Kidding."},
	{"   US-Mexico border\n   Syria"},
	{"   No thanks.\n   Hell yeah!"},
	{"   Let's get out.\n   Hell no"},
	{"   I said no..\n   You're right..."},
	{"   Let's kiss.\n   Let's hurry"},
	{"   Why these places?\n   It was great!"},
	{"   You're a moron\n   I love you"},
	{"   You're a moron\n   I love you"},
};

const unsigned char insersection[11] =
{ 
	7, // Bakaka: Hey,\n how are you doing ?
	9,
	16,
	56, //Grab a gun
	87,
	89,
	91,
	101,
	105,
	108
};

const unsigned char YouMustChoose[140][3] =
{ 
	{0,0},
	{1,1},
	{2,2},
	{3,3},
	{4,4},
	{5,5},
	{6,6},
	{14, 8},	// First choice, 7
	{8,8},
	{10, 13},	// You can't be serious, right ?, 9
	{10,10},
	{11,11},
	{12,12},
	{13,13},
	{14,14},
	{15,15},
	{20, 80},	// Where should we go, 16
	{17,0},
	{18,0},
	{19,0},
	{20,0},
	{21,0},
	{22,0},
	{23,0},
	{24,0},
	{25,0},
	{26,0},
	{27,0},
	{28,0},
	{29,0},
	{30,0},
	{31,0},
	{32,0},
	{33,0},
	{34,0},
	{35,0},
	{36,0},
	{37,0},
	{38,0},
	{39,0},
	{40,0},
	{41,0},
	{42,0},
	{43,0},
	{44,0},
	{45,0},
	{46,0},
	{47,0},
	{48,0},
	{49,0},
	{50,0},
	{51,0},
	{52,0},
	{53,0},
	{54,0},
	{55,0},
	{58,67},	// NO thanks, 56
	{48,0},
	{49,0},
	{50,0},
	{51,0},
	{52,0},
	{53,0},
	{54,0},
	{55,0},
	{48,0},
	{49,0},
	{50,0},
	{51,0},
	{52,0},
	{53,0},
	{54,0},
	{55,0},
	{48,0},
	{49,0},
	{50,0},
	{51,0},
	{52,0},
	{53,0},
	{54,0},
	{55,0},
	{48,0},
	{49,0},
	{50,0},
	{51,0},
	{52,0},
	{0,0},
	{95,88},
	{0,0},
	{90,95},
	{0,0},
	{92,95},
	{49,0},
	{50,0},
	{51,0},
	{52,0},
	{53,0},
	{54,0},
	{55,0},
	{48,0},
	{102,106},
	{102,106},
	{55,0},
	{48,0},
	{109,119},
	{109,119},
	{55,0},
	{109,119},
	{109,119},
};


static unsigned char anybutton_pressed()
{
	if (buttons[0] == 1 || buttons[1] == 1 || buttons[2] == 1)
		return 1;
	return 0;
}

static void Reset_buttons()
{
	buttons[0] = buttons[1] = buttons[2] = 0;
}


static void change_pict(unsigned char pict)
{
	if (remember_back == pict)
		return;
	remember_back = pict;
	
	gbitmap_destroy(current_background);
	
	switch(pict)
	{
		// Titlescreen
		case 254:
		current_background = gbitmap_create_with_resource(RESOURCE_ID_IMAGE_TITLESCREEN);
		break;
		case 0:
		current_background = gbitmap_create_with_resource(RESOURCE_ID_IMAGE_BLACK);
		break;
		case 1:
		current_background = gbitmap_create_with_resource(RESOURCE_ID_IMAGE_BAKURA);
		break;
		case 2:
		current_background = gbitmap_create_with_resource(RESOURCE_ID_IMAGE_TIRED);
		break;
		case 3:
		current_background = gbitmap_create_with_resource(RESOURCE_ID_IMAGE_CLOSING);
		break;
		case 4:
		current_background = gbitmap_create_with_resource(RESOURCE_ID_IMAGE_EVIL);
		break;
		case 5:
		current_background = gbitmap_create_with_resource(RESOURCE_ID_IMAGE_HELL);
		break;
		case 6:
		current_background = gbitmap_create_with_resource(RESOURCE_ID_IMAGE_BLOOD);
		break;
		case 8:
		current_background = gbitmap_create_with_resource(RESOURCE_ID_IMAGE_WORLDMAP);
		break;
		case 9:
		current_background = gbitmap_create_with_resource(RESOURCE_ID_IMAGE_USMEXI_1);
		break;
		case 10:
		current_background = gbitmap_create_with_resource(RESOURCE_ID_IMAGE_USMEXI_2);
		break;
		case 11:
		current_background = gbitmap_create_with_resource(RESOURCE_ID_IMAGE_USMEXI_3);
		break;
		case 12:
		current_background = gbitmap_create_with_resource(RESOURCE_ID_IMAGE_B1);
		break;
		case 13:
		current_background = gbitmap_create_with_resource(RESOURCE_ID_IMAGE_B2);
		break;
		case 14:
		current_background = gbitmap_create_with_resource(RESOURCE_ID_IMAGE_B3);
		break;
		case 15:
		current_background = gbitmap_create_with_resource(RESOURCE_ID_IMAGE_B4);
		break;
		case 16:
		current_background = gbitmap_create_with_resource(RESOURCE_ID_IMAGE_B5);
		break;
		case 17:
		current_background = gbitmap_create_with_resource(RESOURCE_ID_IMAGE_B6);
		break;
		case 18:
		current_background = gbitmap_create_with_resource(RESOURCE_ID_IMG_DESERT);
		break;
		case 19:
		current_background = gbitmap_create_with_resource(RESOURCE_ID_IMAGE_NSC1);
		break;
		case 20:
		current_background = gbitmap_create_with_resource(RESOURCE_ID_IMAGE_B7);
		break;
		case 21:
		current_background = gbitmap_create_with_resource(RESOURCE_ID_IMAGE_B7);
		break;
		case 22:
		current_background = gbitmap_create_with_resource(RESOURCE_ID_IMG_SY1);
		break;
		case 23:
		current_background = gbitmap_create_with_resource(RESOURCE_ID_IMG_SY2);
		break;
		case 24:
		current_background = gbitmap_create_with_resource(RESOURCE_ID_IMG_SY3);
		break;
		case 25:
		current_background = gbitmap_create_with_resource(RESOURCE_ID_IMG_BAKAKA_DICK);
		break;
		case 26:
		current_background = gbitmap_create_with_resource(RESOURCE_ID_IMG_BAKAKA_SUR);
		break;
		case 27:
		current_background = gbitmap_create_with_resource(RESOURCE_ID_IMG_BAKAKA_SUR2);
		break;
		case 28:
		current_background = gbitmap_create_with_resource(RESOURCE_ID_IMG_END1);
		break;
	}
}

static void Generate_enemy_pos(unsigned char tmp)
{
	unsigned char a;
	a = rand_a_b(-1, 3);
	switch(a)
	{
		default:
			x_enn[tmp] = 24;
		break;
		case 1:
			x_enn[tmp] = 64;
		break;
		case 2:
			x_enn[tmp] = 112;
		break;
	}
}

static void Load_minigame_mex()
{
	unsigned char i, a;
	
	change_pict(18);
	bmp[0] = gbitmap_create_with_resource(RESOURCE_ID_IMG_MEX1);
	bmp[1] = gbitmap_create_with_resource(RESOURCE_ID_IMG_MEX2);
	bmp[2] = gbitmap_create_with_resource(RESOURCE_ID_IMG_MEX3);
	bmp[3] = gbitmap_create_with_resource(RESOURCE_ID_IMG_MEX4);
	bmp[4] = gbitmap_create_with_resource(RESOURCE_ID_IMG_GUN1);
	bmp[5] = gbitmap_create_with_resource(RESOURCE_ID_IMG_GUN2);
	
	enemies_number = rand_a_b(90, 110);
	shooting = 0;
	x_man = 32;
	
	for(i=0;i<7;i++)
	{
		Generate_enemy_pos(i);
		y_enn[i] = rand_a_b(-60, -100);
		anim_enn[i] = 0;
		state_enn[i] = 0;
	}
}

static void free_minigame()
{
	unsigned char i;
	for(i=0;i<6;i++)
		gbitmap_destroy(bmp[i]);
}

static void timer_handler(void *context)
{
   layer_mark_dirty(myCanvas);
   app_timer_register(34, timer_handler, NULL);
}

static void config_provider(Window *window)
{
   window_raw_click_subscribe(BUTTON_ID_SELECT, sel_click_handler, sel_release_handler, NULL);
   window_raw_click_subscribe(BUTTON_ID_UP, sel_click_handler_up, sel_release_handler_up, NULL);
   window_raw_click_subscribe(BUTTON_ID_DOWN, sel_click_handler_down, sel_release_handler_down, NULL);
}

static unsigned char Select_Gameover(unsigned char tmp)
{
	if (tmp == 13)
	{
		return 5;
	}
	else if (tmp == 94)
	{
		return 24;
	}
	return 5;
}

static void Back_ToTitlescreen()
{
	Reset_buttons();
	game_mode = 0;
	change_pict(254);
	time_tit = 0;
	progress[0] = 0;	
	progress[1] = 0;	
	titlescreen_y = 0;
	about_to_go = 0;	
	gameover_c = 0;
	titlescreen_press_start_time = 0;
	choice = 0;
	twice = 0;
	about_to_go = 0;
}

static void updateGame(Layer *layer, GContext *ctx)
{
	unsigned char i;
	graphics_context_set_compositing_mode(ctx, GCompOpAssign);
	graphics_context_set_text_color(ctx, GColorBlack);
	
	switch(game_mode)
	{
		// Titlescreen
		case 0:
			graphics_context_set_text_color(ctx, GColorWhite);
			graphics_draw_bitmap_in_rect(ctx, current_background, GRect(0, titlescreen_y, 144, 200));
			graphics_draw_text(ctx, "Western Ideals", fonts_get_system_font(FONT_KEY_ROBOTO_CONDENSED_21), GRect(10, 0, 144, 24), GTextOverflowModeWordWrap, GTextAlignmentLeft, NULL);
			if (titlescreen_y > -10) titlescreen_y--;
			
			switch(about_to_go)
			{
				case 0:
				
					titlescreen_press_start_time++;
					if (titlescreen_press_start_time < 10)
						graphics_draw_text(ctx,"SELECT TO START", fonts_get_system_font(FONT_KEY_GOTHIC_14_BOLD), GRect(20, 144, 168, 24), GTextOverflowModeWordWrap, GTextAlignmentLeft, NULL);
					else if (titlescreen_press_start_time > 20)
						titlescreen_press_start_time = 0;
						
					if (time_tit < 10)
						time_tit++;
						
					if (anybutton_pressed() && time_tit > 8)
					{
						about_to_go = 1;
						time_tit = 0;
					}
				break;
				case 1:
					#ifdef PBL_COLOR
					graphics_context_set_text_color(ctx, GColorRed);
					#else
					graphics_context_set_text_color(ctx, GColorWhite);
					#endif
					
					titlescreen_press_start_time++;
					if (titlescreen_press_start_time < 2)
						graphics_draw_text(ctx,"LET'S GO SUCKA", fonts_get_system_font(FONT_KEY_GOTHIC_14_BOLD), GRect(25, 144, 168, 24), GTextOverflowModeWordWrap, GTextAlignmentLeft, NULL);
					else if (titlescreen_press_start_time > 4)
						titlescreen_press_start_time = 0;
					time_tit++;
					if (time_tit > 40) 
					{
						game_mode = 1;
						progress[0] = 0;
						change_pict(face[progress[0]]);
					}
				break;
			}

			
		break;
		// Ingame
		case 1:
			graphics_draw_bitmap_in_rect(ctx, current_background, GRect(0, 0, 144, 168));
			
			graphics_draw_text(ctx, text[progress[0]], fonts_get_system_font(FONT_KEY_GOTHIC_14), GRect(0, 136, 144, 24), GTextOverflowModeWordWrap, GTextAlignmentLeft, NULL);
			if (anybutton_pressed() && wait_press > 5)
			{
				progress[0]++;
				
				for(i=0;i<sizeof(insersection);i++)
				{
					if (progress[0] == insersection[i])
					{
						game_mode = 2;
						progress[1] = i;
					}
				}
				
				if (text[progress[0]][0] == '*')
					vibes_short_pulse();
				
				// This checks whenever the player has lost, we simply take a look at the first string
				if (text[progress[0]][0] == 'g')
				{
					game_mode = 3;
					change_pict(Select_Gameover(progress[0]));
					vibes_long_pulse();
				}
				else if (text[progress[0]][0] == 'i')
				{
					progress[0] = 21;
					change_pict(face[progress[0]]);
				}
				else if (text[progress[0]][0] == 'q')
				{
					Back_ToTitlescreen();
				}
				else if (text[progress[0]][0] == '^')
				{
					progress[0] = 100;
					change_pict(face[progress[0]]);
				}
				else if (text[progress[0]][0] == 'V')
				{
					game_mode = 5;
					Load_minigame_mex();
				}
				else if (text[progress[0]][0] == '2')
				{
					game_mode = 3;
					gameover_c = 2;
					change_pict(28);
					time_tit = 0;
				}
				/* If not, let's go as usual and change the picture according
				to the text.
				*/
				else
				{
					change_pict(face[progress[0]]);
				}
				
				Reset_buttons();
			}
			if (wait_press < 7) wait_press++;

		break;
		// Choose Your Destiny
		case 2:
			graphics_draw_bitmap_in_rect(ctx, current_background, GRect(0, 0, 144, 168));
			
			graphics_draw_text(ctx, choice_text[progress[1]], fonts_get_system_font(FONT_KEY_GOTHIC_14), GRect(0, 136, 144, 24), GTextOverflowModeWordWrap, GTextAlignmentLeft, NULL);
			graphics_draw_text(ctx, ">", fonts_get_system_font(FONT_KEY_GOTHIC_14), GRect(0, 136+(choice*13), 168, 24), GTextOverflowModeWordWrap, GTextAlignmentLeft, NULL);
			if (buttons[1] == 1) choice =  0;
			else if (buttons[2] == 1) choice =  1;
			// You made your choice and confirm it
			else if (buttons[0] == 1)
			{
				progress[0] = YouMustChoose[progress[0]][choice];
				change_pict(face[progress[0]]);
				Reset_buttons();
				game_mode = 1;
				choice = 0;
			}
		break;
		// You die (depends on how you lose)
		case 3:
			graphics_draw_bitmap_in_rect(ctx, current_background, GRect(0, 0, 144, 168));
			time_tit++;
			graphics_context_set_text_color(ctx, GColorWhite);
				
			if (gameover_c == 1)
				graphics_draw_text(ctx, "Killed by Mexican!", fonts_get_system_font(FONT_KEY_GOTHIC_14), GRect(32, 0, 144, 24), GTextOverflowModeWordWrap, GTextAlignmentLeft, NULL);
				
			if (twice < 4)
			{
				if (time_tit < 2)
				{
					graphics_context_set_fill_color(ctx, GColorWhite);
					graphics_fill_rect(ctx, GRect(0, 0, 144, 168), 0, GCornerNone);
				}
				else if (time_tit > 3)
				{
					twice++;
					time_tit = 0;
				}
			}
			else
			{
				if (time_tit > 50)
				{
					game_mode = 4;
					change_pict(6);
					time_tit = 0;
				}
			}
		break;
		// Game over screen
		case 4:
			graphics_draw_bitmap_in_rect(ctx, current_background, GRect(0, 0, 144, 168));
			#ifdef PBL_COLOR
			graphics_context_set_text_color(ctx, GColorRed);
			#else
			graphics_context_set_text_color(ctx, GColorWhite);
			#endif
			
			if (gameover_c == 2)
				graphics_draw_text(ctx, "BAD ENDING", fonts_get_system_font(FONT_KEY_ROBOTO_CONDENSED_21), GRect(20, 0, 144, 32), GTextOverflowModeWordWrap, GTextAlignmentLeft, NULL);
			else
				graphics_draw_text(ctx, "GAME OVER", fonts_get_system_font(FONT_KEY_ROBOTO_CONDENSED_21), GRect(24, 0, 144, 32), GTextOverflowModeWordWrap, GTextAlignmentLeft, NULL);
			
			time_tit++;
			if ((anybutton_pressed() && time_tit > 15))
			{
				Back_ToTitlescreen();
			}
		break;
		
		/* Shooting at poor innocent Mexicans
		 * (America has gone wrong by this point !) 
		*/
		case 5:
			graphics_draw_bitmap_in_rect(ctx, current_background, GRect(0, 0, 144, 168));
			graphics_context_set_compositing_mode(ctx, GCompOpSet);
			
			if (enemies_number < 1)
			{
				graphics_draw_text(ctx, "You did it !", fonts_get_system_font(FONT_KEY_ROBOTO_CONDENSED_21), GRect(32, 0, 144, 24), GTextOverflowModeWordWrap, GTextAlignmentLeft, NULL);
				time_tit++;	
				if (time_tit > 120)
				{
					progress[0] = 70;
					game_mode = 1;
					free_minigame();
					change_pict(20);
				}
			}
			else
			{
				shooting = 0;
				if (buttons[1])
				{
					x_man = 24;
					shooting = 1;
				}
				else if (buttons[0])
				{
					x_man = 64;
					shooting = 1;
				}
				else if (buttons[2])
				{
					x_man = 112;
					shooting = 1;
				}
				
				for(i=0;i<7;i++)
				{
					if (state_enn[i] == 0)
					{
						if (shooting == 1 && x_man == x_enn[i] && y_enn[i] > 8)
							state_enn[i] = 1;

						anim_enn[i]++;
						if (anim_enn[i] > 2) anim_enn[i] = 0;

						graphics_draw_bitmap_in_rect(ctx, bmp[anim_enn[i]], GRect(x_enn[i], y_enn[i], 16, 16));
						
						if (enemies_number > 50) y_enn[i] += rand_a_b(0,5);
						else y_enn[i] += rand_a_b(3,6);
						
						// Game over man
						if (y_enn[i] > 144)
						{
							game_mode = 3;
							gameover_c = 1;
							time_tit = 0;
							change_pict(19);
						}
					}
					else
					{
						graphics_draw_bitmap_in_rect(ctx, bmp[3], GRect(x_enn[i], y_enn[i], 16, 16));
						state_enn[i]++;
						if (state_enn[i] > 15)
						{
							y_enn[i] = rand_a_b(-20,-60);
							Generate_enemy_pos(i);
							state_enn[i] = 0;
							enemies_number--;
						}
					}
				}
				
				if (shooting == 1)
					graphics_draw_bitmap_in_rect(ctx, bmp[5], GRect(x_man, 152, 16, 16));
				else
					graphics_draw_bitmap_in_rect(ctx, bmp[4], GRect(x_man, 152, 16, 16));
			}
			
			graphics_context_set_compositing_mode(ctx, GCompOpAssign);
		break;
		
	}
							
}

static void init() 
{
   my_window = window_create();
   
   window_set_click_config_provider(my_window, (ClickConfigProvider)config_provider);

   myCanvas = layer_create(GRect(0, 0, 144, 168));

   window_stack_push(my_window, true);
   
   Layer* motherLayer = window_get_root_layer(my_window);
   layer_add_child(motherLayer, myCanvas);

   layer_set_update_proc(myCanvas, updateGame);
   app_timer_register(34, timer_handler, NULL);
}

static void deinit() 
{
   text_layer_destroy(text_layer);
   window_destroy(my_window);
}

int main(void) 
{
	Back_ToTitlescreen();
	init();
	app_event_loop();
	deinit();
}
