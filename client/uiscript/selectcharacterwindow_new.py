import uiScriptLocale

ROOT_PATH = "d:/ymir work/ui/public/"
LOCALE_PATH = uiScriptLocale.SELECT_PATH

BOARD_X = SCREEN_WIDTH * (20) / 800
BOARD_Y = SCREEN_HEIGHT * (220) / 600

SIDEBAR_Y = 78

BOARD_ITEM_ADD_POSITION = 10
BOARD_ITEM_ADD_X = 20
BOARD_ITEM_ADD_GAUGE_X = 10

window = {
	"name" : "SelectCharacterWindow",

	"x" : 0,
	"y" : 0,

	"width" : SCREEN_WIDTH,
	"height" : SCREEN_HEIGHT,

	"children" :
	(
		## Board
		{
			"name" : "BackGround", 
			"type" : "window", 
			
			"x" : 0,
			"y" : 0,
			
			"width" : SCREEN_WIDTH, 
			"height" : SCREEN_HEIGHT,
		},
		## Character Board
		{
			"name" : "character_board",
			"type" : "thinboard",

			"x" : BOARD_X,
			"y" : BOARD_Y,

			"width" : 208,
			"height" : 393 + BOARD_ITEM_ADD_POSITION,

			"children" :
				(

					## Empire Flag
				{
					"name" : "EmpireFlag_A",
					"type" : "expanded_image",

					"x" : 21,
					"y" : 12,
					"x_scale" : 0.5,
					"y_scale" : 0.5,

					"image" : "d:/ymir work/ui/intro/empire/empireflag_a.sub"
				},
				{
					"name" : "EmpireFlag_B",
					"type" : "expanded_image",

					"x" : 21,
					"y" : 12,
					"x_scale" : 0.5,
					"y_scale" : 0.5,

					"image" : "d:/ymir work/ui/intro/empire/empireflag_b.sub"
				},
				{
					"name" : "EmpireFlag_C",
					"type" : "expanded_image",

					"x" : 21,
					"y" : 12,
					"x_scale" : 0.5,
					"y_scale" : 0.5,

					"image" : "d:/ymir work/ui/intro/empire/empireflag_c.sub"
				},

				{
					"name" : "EmpireNameSlot",
					"type" : "image",

					"x" : 100,
					"y" : 12,

					"image" : "d:/ymir work/ui/public/Parameter_Slot_03.sub",

					"children" :
					(
						{
							"name" : "EmpireName",
							"type" : "text",

							"x" : 0,
							"y" : 0,

							"text" : uiScriptLocale.SELECT_EMPIRE_NAME,

							"all_align" : "center",
						},
					),
				},

				{
					"name" : "GuildNameSlot",
					"type" : "image",

					"x" : 100,
					"y" : 33,

					"image" : "d:/ymir work/ui/public/Parameter_Slot_03.sub",

					"children" :
					(
						{
							"name" : "GuildName",
							"type" : "text",

							"x" : 0,
							"y" : 0,

							"text" : uiScriptLocale.SELECT_NO_GUILD,

							"all_align" : "center",
						},
					),
				},

				{
					"name" : "character_name",
					"type" : "text",

					"x" : 17,
					"y" : 124 - 21 + BOARD_ITEM_ADD_POSITION,

					"text" : uiScriptLocale.SELECT_NAME,

					"children" :
					(
						{
							"name" : "character_name_slot",
							"type" : "image",

							"x" : 43,
							"y" : -2,

							"image" : "d:/ymir work/ui/public/Parameter_Slot_05.sub",
						},
						{
							"name" : "character_name_value",
							"type" : "text",

							"x" : 43 + 130/2,
							"y" : 0,

							"text" : "",

							"text_horizontal_align" : "center",
						},
					),
				},
				{
					"name" : "character_level",
					"type" : "text",

					"x" : 17,
					"y" : 50 + 100 - 21 + BOARD_ITEM_ADD_POSITION,

					"text" : uiScriptLocale.SELECT_LEVEL,

					"children" :
					(
						{
							"name" : "character_level_slot",
							"type" : "image",

							"x" : 43,
							"y" : -2,

							"image" : "d:/ymir work/ui/public/Parameter_Slot_05.sub",
						},
						{
							"name" : "character_level_value",
							"type" : "text",

							"x" : 43 + 130/2,
							"y" : 0,

							"text" : "",

							"text_horizontal_align" : "center",
						},
					),
				},
				{
					"name" : "character_play_time",
					"type" : "text",

					"x" : 17,
					"y" : 76 + 100 - 21 + BOARD_ITEM_ADD_POSITION,

					"text" : uiScriptLocale.SELECT_PLAYTIME,

					"children" :
					(
						{
							"name" : "character_play_time_slot",
							"type" : "image",

							"x" : 83,
							"y" : -2,

							"image" : "d:/ymir work/ui/public/Parameter_Slot_03.sub",
						},
						{
							"name" : "character_play_time_value",
							"type" : "text",

							"x" : 83 + 91/2,
							"y" : 0,

							"text" : "",

							"text_horizontal_align" : "center",
						},
					),
				},
				{
					"name" : "character_hth",
					"type" : "text",

					"x" : 17,
					"y" : 102 + 100 - 21 + BOARD_ITEM_ADD_POSITION,

					"text" : uiScriptLocale.SELECT_HP,

					"children" :
					(
						{
							"name" : "gauge_hth",
							"type" : "gauge",

							"x" : 30,
							"y" : 4,

							"width" : 100,
							"color" : "red",
						},
						{
							"name" : "character_hth_slot",
							"type" : "image",

							"x" : 134,
							"y" : -2,

							"image" : "d:/ymir work/ui/public/Parameter_Slot_00.sub",
						},
						{
							"name" : "character_hth_value",
							"type" : "text",

							"x" : 134 + 39/2,
							"y" : 0,

							"text" : "",

							"text_horizontal_align" : "center",
						},
					),
				},
				{
					"name" : "character_int",
					"type" : "text",

					"x" : 17,
					"y" : 128 + 100 - 21 + BOARD_ITEM_ADD_POSITION,

					"text" : uiScriptLocale.SELECT_SP,

					"children" :
					(
						{
							"name" : "gauge_int",
							"type" : "gauge",

							"x" : 30,
							"y" : 4,

							"width" : 100,
							"color" : "pink",
						},
						{
							"name" : "character_int_slot",
							"type" : "image",

							"x" : 134,
							"y" : -2,

							"image" : "d:/ymir work/ui/public/Parameter_Slot_00.sub",
						},
						{
							"name" : "character_int_value",
							"type" : "text",

							"x" : 134 + 39/2,
							"y" : 0,

							"text" : "",

							"text_horizontal_align" : "center",
						},
					),
				},
				{
					"name" : "character_str",
					"type" : "text",

					"x" : 17,
					"y" : 154 + 100 - 21 + BOARD_ITEM_ADD_POSITION,

					"text" : uiScriptLocale.SELECT_ATT_GRADE,

					"children" :
					(
						{
							"name" : "gauge_str",
							"type" : "gauge",

							"x" : 30,
							"y" : 4,

							"width" : 100,
							"color" : "purple",
						},
						{
							"name" : "character_str_slot",
							"type" : "image",

							"x" : 134,
							"y" : -2,

							"image" : "d:/ymir work/ui/public/Parameter_Slot_00.sub",
						},
						{
							"name" : "character_str_value",
							"type" : "text",

							"x" : 134 + 39/2,
							"y" : 0,

							"text" : "",

							"text_horizontal_align" : "center",
						},
					),
				},
				{
					"name" : "character_dex",
					"type" : "text",

					"x" : 17,
					"y" : 180 + 100 - 21 + BOARD_ITEM_ADD_POSITION,

					"text" : uiScriptLocale.SELECT_DEX_GRADE,

					"children" :
					(
						{
							"name" : "gauge_dex",
							"type" : "gauge",

							"x" : 30,
							"y" : 4,

							"width" : 100,
							"color" : "blue",
						},
						{
							"name" : "character_dex_slot",
							"type" : "image",

							"x" : 134,
							"y" : -2,

							"image" : "d:/ymir work/ui/public/Parameter_Slot_00.sub",
						},
						{
							"name" : "character_dex_value",
							"type" : "text",

							"x" : 134 + 39/2,
							"y" : 0,

							"text" : "",

							"text_horizontal_align" : "center",
						},
					),
				},

				## Buttons
				{
					"name" : "start_button",
					"type" : "button",

					"x" : 14,
					"y" : 210 + 100 - 21 + BOARD_ITEM_ADD_POSITION,

					"text" : uiScriptLocale.SELECT_SELECT,
					"text_height" : 6,

					"default_image" : ROOT_PATH + "XLarge_Button_01.sub",
					"over_image" : ROOT_PATH + "XLarge_Button_02.sub",
					"down_image" : ROOT_PATH + "XLarge_Button_03.sub",
				},
				{
					"name" : "create_button",
					"type" : "button",

					"x" : 14,
					"y" : 210 + 100 - 21 + BOARD_ITEM_ADD_POSITION,

					"text" : uiScriptLocale.SELECT_CREATE,
					"text_height" : 6,

					"default_image" : ROOT_PATH + "XLarge_Button_01.sub",
					"over_image" : ROOT_PATH + "XLarge_Button_02.sub",
					"down_image" : ROOT_PATH + "XLarge_Button_03.sub",
				},
				{
					"name" : "rotLeft",
					"type" : "button",
					
					"x" : 14,
					"y" : 247 + 100 - 21 + BOARD_ITEM_ADD_POSITION,

					"text" : "Links drehen",
					
					"default_image" : ROOT_PATH + "Large_Button_01.sub",
					"over_image" : ROOT_PATH + "Large_Button_02.sub",
					"down_image" : ROOT_PATH + "Large_Button_03.sub",
				},
				
				{
					"name" : "rotRight",
					"type" : "button",
					
					"x" : 105,
					"y" : 247 + 100 - 21 + BOARD_ITEM_ADD_POSITION,

					"default_image" : ROOT_PATH + "Large_Button_01.sub",
					"over_image" : ROOT_PATH + "Large_Button_02.sub",
					"down_image" : ROOT_PATH + "Large_Button_03.sub",
					
					"text" : "Rechts drehen",
				},
				{
					"name" : "exit_button",
					"type" : "button",

					"x" : 105,
					"y" : 280 + 100 - 21 + BOARD_ITEM_ADD_POSITION,

					"text" : uiScriptLocale.SELECT_EXIT,

					"default_image" : ROOT_PATH + "Large_Button_01.sub",
					"over_image" : ROOT_PATH + "Large_Button_02.sub",
					"down_image" : ROOT_PATH + "Large_Button_03.sub",
				},
				{
					"name" : "delete_button",
					"type" : "button",

					"x" : 14,
					"y" : 280 + 100 - 21 + BOARD_ITEM_ADD_POSITION,

					"text" : uiScriptLocale.SELECT_DELETE,

					"default_image" : ROOT_PATH + "Large_Button_01.sub",
					"over_image" : ROOT_PATH + "Large_Button_02.sub",
					"down_image" : ROOT_PATH + "Large_Button_03.sub",
				},
				
			),
		},
		
				## SelectBoard
		{
			"name" : "SelectBoard",
			"type" : "board",
			
			"width" : 800,
			"height" : 60,

			"x" : 0,
			"y" : SCREEN_HEIGHT - 50,
			
			"horizontal_align" : "center",
			
			"children" : 
			(
				## Buttons
				{
					"name" : "char1_button",
					"type" : "radio_button",

					"x" : 52 + 152*0,
					"y" : 15,

					"default_image" : ROOT_PATH + "Large_Button_01.sub",
					"over_image" : ROOT_PATH + "Large_Button_02.sub",
					"down_image" : ROOT_PATH + "Large_Button_03.sub",
					
					"text" : "Leerer Slot",
				},
				{
					"name" : "char2_button",
					"type" : "radio_button",
					
					"x" : 52 + 152*1,
					"y" : 15,

					"default_image" : ROOT_PATH + "Large_Button_01.sub",
					"over_image" : ROOT_PATH + "Large_Button_02.sub",
					"down_image" : ROOT_PATH + "Large_Button_03.sub",
					
					"text" : "Leerer Slot",
				},
				{
					"name" : "char3_button",
					"type" : "radio_button",

					"x" : 52 + 152*2,
					"y" : 15,

					"default_image" : ROOT_PATH + "Large_Button_01.sub",
					"over_image" : ROOT_PATH + "Large_Button_02.sub",
					"down_image" : ROOT_PATH + "Large_Button_03.sub",
					
					"text" : "Leerer Slot",
				},
				{
					"name" : "char4_button",
					"type" : "radio_button",
					
					"x" : 52 + 152*3,
					"y" : 15,

					"default_image" : ROOT_PATH + "Large_Button_01.sub",
					"over_image" : ROOT_PATH + "Large_Button_02.sub",
					"down_image" : ROOT_PATH + "Large_Button_03.sub",
					
					"text" : "Leerer Slot",
				},
				{
					"name" : "char5_button",
					"type" : "radio_button",
					
					"x" : 52 + 152*4,
					"y" : 15,

					"default_image" : ROOT_PATH + "Large_Button_01.sub",
					"over_image" : ROOT_PATH + "Large_Button_02.sub",
					"down_image" : ROOT_PATH + "Large_Button_03.sub",
					
					"text" : "Leerer Slot",
				},
			),
		},
	),
}
