import chr
import grp
import app
import math
import wndMgr
import snd
import net
import systemSetting
import localeInfo
import chr
import dbg
import chrmgr
import background

import ui
import uiScriptLocale
import networkModule
import musicInfo
import playerSettingModule

import uiCommon                    
import uiMapNameShower             
import uiAffectShower              
import uiPlayerGauge               
import uiCharacter                 
import uiTarget                    
import consoleModule               

import interfaceModule
import uiTaskBar                   
import uiInventory

###################################

LEAVE_BUTTON_FOR_POTAL = False
NOT_NEED_DELETE_CODE = False
ENABLE_ENGNUM_DELETE_CODE = False

###################################

class SelectCharacterWindow(ui.Window):

	SLOT_ROTATION = [135.0, 225.0, 315.0, 45.0, 405.0]
	SLOT_COUNT = 5
	CHARACTER_TYPE_COUNT = 4

	INSTANCE_POSITIONS = { 	
							0 : { "x" : 13100.100000,"y" : 11750.6000000, "z" : 16378.9997101, "rot" : 135.0}, 
							1 : { "x" : 13225.100000,"y" : 11950.6000000, "z" : 16378.9997101, "rot" : 135.0}, 
							2 : { "x" : 13375.100000,"y" : 12150.6000000, "z" : 16378.9997101, "rot" : 135.0}, 
							3 : { "x" : 13500.100000,"y" : 12350.6000000, "z" : 16378.9997101, "rot" : 135.0}, 
							4 : { "x" : 13575.100000,"y" : 12550.6000000, "z" : 16378.9997101, "rot" : 135.0}, 
							"dest" : { "x" : 12688.0002022, "y" : 12868.0002213, "rot" : 135.0}
						}
	
	EMPIRE_NAME = {
		net.EMPIRE_A : localeInfo.EMPIRE_A,
		net.EMPIRE_B : localeInfo.EMPIRE_B,
		net.EMPIRE_C : localeInfo.EMPIRE_C
	}

	def  OnRender(self):
		chr.Render()
		app.RenderGame()
		grp.PopState()
		grp.SetInterfaceRenderState()

	def __init__(self, stream):
		ui.Window.__init__(self)
		net.SetPhaseWindow(net.PHASE_WINDOW_SELECT, self)

		self.stream=stream
		self.slot = self.stream.GetCharacterSlot()

		self.openLoadingFlag = False
		self.startIndex = -1
		self.startReservingTime = 0

		self.flagDict = {}
		self.curRotation = []
		self.destRotation = []
		for rot in self.SLOT_ROTATION:
			self.curRotation.append(rot)
			self.destRotation.append(rot)

		self.curNameAlpha = []
		self.destNameAlpha = []
		for i in xrange(self.CHARACTER_TYPE_COUNT):
			self.curNameAlpha.append(0.0)
			self.destNameAlpha.append(0.0)

		self.curGauge = [0.0, 0.0, 0.0, 0.0]
		self.destGauge = [0.0, 0.0, 0.0, 0.0]

		self.dlgBoard = 0
		self.changeNameFlag = False
		self.nameInputBoard = None
		self.sendedChangeNamePacket = False

		self.startIndex = -1
		self.isLoad = 0
		
		self.MainRotationHasChanges = {"index" : 0, "changes" : False, "rot" : self.INSTANCE_POSITIONS["dest"]["rot"]}

	def __del__(self):
		ui.Window.__del__(self)
		net.SetPhaseWindow(net.PHASE_WINDOW_SELECT, 0)

	def Open(self):

		playerSettingModule.LoadGameData("INIT")
		playerSettingModule.LoadGameData("EFFECT")
		playerSettingModule.LoadGameData("SHAMAN")
		playerSettingModule.LoadGameData("SURA")
		playerSettingModule.LoadGameData("ASSASSIN")
		playerSettingModule.LoadGameData("WARRIOR")
	
		if not self.__LoadBoardDialog("uiscript/selectcharacterwindow_new.py"):
			import dbg
			dbg.TraceError("SelectCharacterWindow.Open - __LoadScript Error")
			return

		if not self.__LoadQuestionDialog("uiscript/questiondialog.py"):
			return

		self.InitCharacterBoard()

		self.btnStart.Enable()
		self.btnCreate.Enable()
		self.btnDelete.Enable()
		self.btnExit.Enable()

		self.dlgBoard.Show()
		self.SetWindowName("SelectCharacterWindow")
		self.Show()

		if musicInfo.selectMusic != "":
			snd.SetMusicVolume(systemSetting.GetMusicVolume())
			snd.FadeInMusic("BGM/"+musicInfo.selectMusic)

		# app.SetCenterPosition(0.0, 0.0, 0.0)
		# app.SetCamera(1550.0, 15.0, 180.0, 95.0)

		self.isLoad=1
		self.Refresh()

		self.HideAllFlag()
		self.SetEmpire(net.GetEmpireID())

		app.ShowCursor()
		# self.StartGame()
		background.Initialize()
		app.SetCamera(453.0,0.0,46.0,200.0)
		net.Warp(831397,13368)
		background.RegisterEnvironmentData(2, "d:/ymir work/environment/bayblacksand.msenv")
		background.SetEnvironmentData(2)
		
		for i in xrange(self.SLOT_COUNT):

			if False == chr.HasInstance(i):
				continue

			chr.SelectInstance(i)
			chr.SetPixelPosition(self.INSTANCE_POSITIONS[i]["x"], self.INSTANCE_POSITIONS[i]["y"], self.INSTANCE_POSITIONS[i]["z"])
			chr.SetRotation(self.INSTANCE_POSITIONS[i]["rot"])
			#chr.LookAtDestPixelPosition(self.INSTANCE_POSITIONS["look_dest"]["x"], self.INSTANCE_POSITIONS["look_dest"]["y"])
			
			
			
			if self.slot == i:
				self.SelectSlot(self.slot)
				
				
		self.SetFocus()

		
	def Close(self):
		if musicInfo.selectMusic != "":
			snd.FadeOutMusic("BGM/"+musicInfo.selectMusic)

		self.stream.popupWindow.Close()

		if self.dlgBoard:
			self.dlgBoard.ClearDictionary()

		self.flagDict = {}
		self.dlgBoard = None
		self.btnStart = None
		self.btnCreate = None
		self.btnDelete = None
		self.btnExit = None
		self.backGround = None
		self.empireName = None
		
		self.charButtons = []

		self.dlgQuestion.ClearDictionary()
		self.dlgQuestion = None
		self.dlgQuestionText = None
		self.dlgQuestionAcceptButton = None
		self.dlgQuestionCancelButton = None
		self.privateInputBoard = None
		self.nameInputBoard = None

		chr.DeleteInstance(0)
		chr.DeleteInstance(1)
		chr.DeleteInstance(2)
		chr.DeleteInstance(3)

		self.Hide()
		self.KillFocus()

		app.HideCursor()

	def SetEmpire(self, id):
		self.empireName.SetText(self.EMPIRE_NAME.get(id, ""))
		if self.flagDict.has_key(id):
			self.flagDict[id].Show()
		
	def HideAllFlag(self):
		for flag in self.flagDict.values():
			flag.Hide()

	def Refresh(self):
		if not self.isLoad:
			return

		# SLOT4
		indexArray = (4, 3, 2, 1, 0)
		for index in indexArray:
			id=net.GetAccountCharacterSlotDataInteger(index, net.ACCOUNT_CHARACTER_SLOT_ID)
			race=net.GetAccountCharacterSlotDataInteger(index, net.ACCOUNT_CHARACTER_SLOT_RACE)
			form=net.GetAccountCharacterSlotDataInteger(index, net.ACCOUNT_CHARACTER_SLOT_FORM)
			name=net.GetAccountCharacterSlotDataString(index, net.ACCOUNT_CHARACTER_SLOT_NAME)
			hair=net.GetAccountCharacterSlotDataInteger(index, net.ACCOUNT_CHARACTER_SLOT_HAIR)
			weapon=net.GetAccountCharacterSlotDataInteger(index, net.ACCOUNT_CHARACTER_SLOT_WEAPON)
			acce=net.GetAccountCharacterSlotDataInteger(index, net.ACCOUNT_CHARACTER_SLOT_ACCE) ##ENABLE_ACCE_SYSTEM

			if id:
				self.MakeCharacter(index, id, name, race, form, hair, weapon, acce) ##ENABLE_ACCE_SYSTEM
				self.SelectSlot(index)

		self.SelectSlot(self.slot)

	def GetCharacterSlotID(self, slotIndex):
		return net.GetAccountCharacterSlotDataInteger(slotIndex, net.ACCOUNT_CHARACTER_SLOT_ID)

	def __LoadQuestionDialog(self, fileName):
		self.dlgQuestion = ui.ScriptWindow()

		try:
			pyScrLoader = ui.PythonScriptLoader()
			pyScrLoader.LoadScriptFile(self.dlgQuestion, fileName)
		except:
			import exception
			exception.Abort("SelectCharacterWindow.LoadQuestionDialog.LoadScript")

		try:
			GetObject=self.dlgQuestion.GetChild
			self.dlgQuestionText=GetObject("message")
			self.dlgQuestionAcceptButton=GetObject("accept")
			self.dlgQuestionCancelButton=GetObject("cancel")
		except:
			import exception
			exception.Abort("SelectCharacterWindow.LoadQuestionDialog.BindObject")

		self.dlgQuestionText.SetText(localeInfo.SELECT_DO_YOU_DELETE_REALLY)
		self.dlgQuestionAcceptButton.SetEvent(ui.__mem_func__(self.RequestDeleteCharacter))
		self.dlgQuestionCancelButton.SetEvent(ui.__mem_func__(self.dlgQuestion.Hide))
		return 1

	def __LoadBoardDialog(self, fileName):
		self.dlgBoard = ui.ScriptWindow()

		try:
			pyScrLoader = ui.PythonScriptLoader()
			pyScrLoader.LoadScriptFile(self.dlgBoard, fileName)
		except:
			import exception
			exception.Abort("SelectCharacterWindow.LoadBoardDialog.LoadScript")

		try:
			GetObject=self.dlgBoard.GetChild

			self.btnStart		= GetObject("start_button")
			self.btnCreate		= GetObject("create_button")
			self.btnDelete		= GetObject("delete_button")
			self.btnExit		= GetObject("exit_button")

			self.CharacterName	= GetObject("character_name_value")
			self.CharacterLevel = GetObject("character_level_value")
			self.PlayTime		= GetObject("character_play_time_value")
			self.CharacterHTH	= GetObject("character_hth_value")
			self.CharacterINT	= GetObject("character_int_value")
			self.CharacterSTR	= GetObject("character_str_value")
			self.CharacterDEX	= GetObject("character_dex_value")
			self.GuildName		= GetObject("GuildName")

			self.GaugeList = []
			self.GaugeList.append(GetObject("gauge_hth"))
			self.GaugeList.append(GetObject("gauge_int"))
			self.GaugeList.append(GetObject("gauge_str"))
			self.GaugeList.append(GetObject("gauge_dex"))

			self.charButtons = []
			self.charButtons.append(GetObject("char1_button"))
			self.charButtons.append(GetObject("char2_button"))
			self.charButtons.append(GetObject("char3_button"))
			self.charButtons.append(GetObject("char4_button"))
			self.charButtons.append(GetObject("char5_button"))
			
			self.empireName = GetObject("EmpireName")
			self.flagDict[net.EMPIRE_A] = GetObject("EmpireFlag_A")
			self.flagDict[net.EMPIRE_B] = GetObject("EmpireFlag_B")
			self.flagDict[net.EMPIRE_C] = GetObject("EmpireFlag_C")
			
			self.btnRotationLeft = GetObject("rotLeft")
			self.btnRotationRight = GetObject("rotRight")

			self.backGround = GetObject("BackGround")

		except:
			import exception
			exception.Abort("SelectCharacterWindow.LoadBoardDialog.BindObject")

		self.btnStart.SetEvent(ui.__mem_func__(self.StartGame))
		self.btnCreate.SetEvent(ui.__mem_func__(self.CreateCharacter))
		self.btnExit.SetEvent(ui.__mem_func__(self.ExitSelect))

		

		if NOT_NEED_DELETE_CODE:
			self.btnDelete.SetEvent(ui.__mem_func__(self.PopupDeleteQuestion))
		else:
			self.btnDelete.SetEvent(ui.__mem_func__(self.InputPrivateCode))

		self.charButtons[0].SetEvent(lambda arg=0: self.SelectSlot(arg))
		self.charButtons[1].SetEvent(lambda arg=1: self.SelectSlot(arg))
		self.charButtons[2].SetEvent(lambda arg=2: self.SelectSlot(arg))
		self.charButtons[3].SetEvent(lambda arg=3: self.SelectSlot(arg))
		self.charButtons[4].SetEvent(lambda arg=4: self.SelectSlot(arg))
		
		# self.btnRotationLeft.SetEvent(lambda arg= "left": self.Rotate(arg))
		# self.btnRotationRight.SetEvent(lambda arg= "right": self.Rotate(arg))
		

		# self.chrRenderer = self.CharacterRenderer()
		# self.chrRenderer.SetParent(self.backGround)
		# self.chrRenderer.Show()
		
		return 1
    
	def SameLoginDisconnect(self):
		self.stream.popupWindow.Close()
		self.stream.popupWindow.Open(localeInfo.LOGIN_FAILURE_SAMELOGIN, self.ExitSelect, localeInfo.UI_OK)
        #self.stream.popupWindow.Open(localeInfo.LOGIN_FAILURE_ALREAY, app.Exit, localeInfo.UI_OK)
        
	def MakeCharacter(self, index, id, name, race, form, hair, weapon, acce): ##ENABLE_ACCE_SYSTEM
		if 0 == id:
			return

		chr.CreateInstance(index)
		chr.SelectInstance(index)
		chr.SetVirtualID(index)
		chr.SetNameString(name)
		self.charButtons[index].SetText(name)
		chr.SetInstanceType(chr.INSTANCE_TYPE_PLAYER)

	#	chr.SetCanSelect(0)

		chr.SetRace(race)
		chr.SetArmor(form)
		chr.SetHair(hair)
		chr.SetWeapon(weapon)
		chr.SetAcce(acce) ##ENABLE_ACCE_SYSTEM

		#chr.SetMotionMode(chr.MOTION_MODE_GENERAL)
		#chr.SetLoopMotion(chr.MOTION_WAIT)

		chr.SetRotation(self.INSTANCE_POSITIONS[index]["rot"])

	## Manage Character
	def StartGame(self):

		if self.sendedChangeNamePacket:
			return

		if self.changeNameFlag:
			self.OpenChangeNameDialog()
			return

		if -1 != self.startIndex:
			return

		if musicInfo.selectMusic != "":
			snd.FadeLimitOutMusic("BGM/"+musicInfo.selectMusic, systemSetting.GetMusicVolume()*0.05)

		self.btnStart.SetUp()
		self.btnCreate.SetUp()
		self.btnDelete.SetUp()
		self.btnExit.SetUp()
		self.btnRotationLeft.SetUp()
		self.btnRotationRight.SetUp()

		self.btnStart.Disable()
		self.btnCreate.Disable()
		self.btnDelete.Disable()
		self.btnExit.Disable()
		self.btnRotationLeft.Disable()
		self.btnRotationRight.Disable()
		self.dlgQuestion.Hide()

		self.stream.SetCharacterSlot(self.slot)

		self.startIndex = self.slot
		self.startReservingTime = app.GetTime()

		for i in xrange(self.SLOT_COUNT):

			if False == chr.HasInstance(i):
				continue

			chr.SelectInstance(i)

			if i == self.slot:
				self.slot=self.slot
				chr.PushOnceMotion(chr.MOTION_INTRO_SELECTED, 0.1)
				continue

			chr.PushOnceMotion(chr.MOTION_INTRO_NOT_SELECTED, 0.1)

	def OpenChangeNameDialog(self):
		import uiCommon
		nameInputBoard = uiCommon.InputDialogWithDescription()
		nameInputBoard.SetTitle(localeInfo.SELECT_CHANGE_NAME_TITLE)
		nameInputBoard.SetAcceptEvent(ui.__mem_func__(self.AcceptInputName))
		nameInputBoard.SetCancelEvent(ui.__mem_func__(self.CancelInputName))
		nameInputBoard.SetMaxLength(chr.PLAYER_NAME_MAX_LEN)
		nameInputBoard.SetBoardWidth(200)
		nameInputBoard.SetDescription(localeInfo.SELECT_INPUT_CHANGING_NAME)
		nameInputBoard.Open()
		nameInputBoard.slot = self.slot
		self.nameInputBoard = nameInputBoard

	def OnChangeName(self, id, name):
		self.SelectSlot(id)
		self.sendedChangeNamePacket = False
		self.PopupMessage(localeInfo.SELECT_CHANGED_NAME)

	def AcceptInputName(self):
		changeName = self.nameInputBoard.GetText()
		if not changeName:
			return

		self.sendedChangeNamePacket = True
		net.SendChangeNamePacket(self.nameInputBoard.slot, changeName)
		return self.CancelInputName()

	def CancelInputName(self):
		self.nameInputBoard.Close()
		self.nameInputBoard = None
		return True

	def OnCreateFailure(self, type):
		self.sendedChangeNamePacket = False
		if 0 == type:
			self.PopupMessage(localeInfo.SELECT_CHANGE_FAILURE_STRANGE_NAME)
		elif 1 == type:
			self.PopupMessage(localeInfo.SELECT_CHANGE_FAILURE_ALREADY_EXIST_NAME)
		elif 100 == type:
			self.PopupMessage(localeInfo.SELECT_CHANGE_FAILURE_STRANGE_INDEX)

	def CreateCharacter(self):
		id = self.GetCharacterSlotID(self.slot)
		if 0==id:
			self.stream.SetCharacterSlot(self.slot)

			EMPIRE_MODE = 1

			if EMPIRE_MODE:
				if self.__AreAllSlotEmpty():
					self.stream.SetReselectEmpirePhase()
				else:
					self.stream.SetCreateCharacterPhase()

			else:
				self.stream.SetCreateCharacterPhase()

	def __AreAllSlotEmpty(self):
		for iSlot in xrange(self.SLOT_COUNT):
			if 0!=net.GetAccountCharacterSlotDataInteger(iSlot, net.ACCOUNT_CHARACTER_SLOT_ID):
				return 0
		return 1

	def PopupDeleteQuestion(self):
		id = self.GetCharacterSlotID(self.slot)
		if 0 == id:
			return

		self.dlgQuestion.Show()
		self.dlgQuestion.SetTop()

	def RequestDeleteCharacter(self):
		self.dlgQuestion.Hide()

		id = self.GetCharacterSlotID(self.slot)
		if 0 == id:
			self.PopupMessage(localeInfo.SELECT_EMPTY_SLOT)
			return

		net.SendDestroyCharacterPacket(self.slot, "1234567")
		self.PopupMessage(localeInfo.SELECT_DELEING)
		
		# self.charButtons[index].SetText("Leerer Slot")
		# self.Refresh()

	def InputPrivateCode(self):
		
		import uiCommon
		privateInputBoard = uiCommon.InputDialogWithDescription()
		privateInputBoard.SetTitle(localeInfo.INPUT_PRIVATE_CODE_DIALOG_TITLE)
		privateInputBoard.SetAcceptEvent(ui.__mem_func__(self.AcceptInputPrivateCode))
		privateInputBoard.SetCancelEvent(ui.__mem_func__(self.CancelInputPrivateCode))

		if ENABLE_ENGNUM_DELETE_CODE:
			pass
		else:
			privateInputBoard.SetNumberMode()

		privateInputBoard.SetSecretMode()
		privateInputBoard.SetMaxLength(7)
			
		privateInputBoard.SetBoardWidth(250)
		privateInputBoard.SetDescription(localeInfo.INPUT_PRIVATE_CODE_DIALOG_DESCRIPTION)
		privateInputBoard.Open()
		self.privateInputBoard = privateInputBoard

	def AcceptInputPrivateCode(self):
		privateCode = self.privateInputBoard.GetText()
		if not privateCode:
			return

		id = self.GetCharacterSlotID(self.slot)
		if 0 == id:
			self.PopupMessage(localeInfo.SELECT_EMPTY_SLOT)
			return

		net.SendDestroyCharacterPacket(self.slot, privateCode)
		self.PopupMessage(localeInfo.SELECT_DELEING)

		self.CancelInputPrivateCode()
		return True

	def CancelInputPrivateCode(self):
		self.privateInputBoard = None
		return True

	def OnDeleteSuccess(self, slot):
		self.PopupMessage(localeInfo.SELECT_DELETED)
		self.DeleteCharacter(slot)

	def OnDeleteFailure(self):
		self.PopupMessage(localeInfo.SELECT_CAN_NOT_DELETE)

	def DeleteCharacter(self, index):
		chr.DeleteInstance(index)
		self.charButtons[index].SetText("Leerer Slot")
		self.SelectSlot(self.slot)

	def ExitSelect(self):
		self.dlgQuestion.Hide()
	
		if LEAVE_BUTTON_FOR_POTAL:
			if app.loggined:
				self.stream.SetPhaseWindow(0)
			else:
				self.stream.setloginphase()
		else:
			self.stream.SetLoginPhase()

		self.Hide()

	def GetSlotIndex(self):
		return self.slot

	def DecreaseSlotIndex(self):
		slotIndex = (self.GetSlotIndex() - 1 + self.SLOT_COUNT) % self.SLOT_COUNT
		self.SelectSlot(slotIndex)

	def IncreaseSlotIndex(self):
		slotIndex = (self.GetSlotIndex() + 1) % self.SLOT_COUNT
		self.SelectSlot(slotIndex)

	def SelectSlot(self, index):

		if index < 0:
			return
		if index >= self.SLOT_COUNT:
			return
			
		self.MainRotationHasChanges["index"] = index
		self.MainRotationHasChanges["changes"] = False
		self.MainRotationHasChanges["rot"] = self.INSTANCE_POSITIONS["dest"]["rot"]
		
		for x in xrange(self.SLOT_COUNT):
			if x != index:
				self.charButtons[x].SetUp()
			elif x == index:
				self.charButtons[x].Down()

		self.slot = index
		
		for i in range(self.SLOT_COUNT):
			if chr.HasInstance(i):
				chr.SelectInstance(i)
				if i == index:
					chr.MoveToDestPosition(i,self.INSTANCE_POSITIONS["dest"]["x"], self.INSTANCE_POSITIONS["dest"]["y"])				
				else:
					chr.MoveToDestPosition(i,self.INSTANCE_POSITIONS[i]["x"], self.INSTANCE_POSITIONS[i]["y"])
				
		chr.SelectInstance(self.slot)

		for i in xrange(self.CHARACTER_TYPE_COUNT):
			self.destNameAlpha[i] = 0.0

		for i in xrange(self.SLOT_COUNT):
			self.destRotation[(i+self.slot)%self.SLOT_COUNT] = self.SLOT_ROTATION[i]

		self.destGauge = [0.0, 0.0, 0.0, 0.0]

		id=net.GetAccountCharacterSlotDataInteger(self.slot, net.ACCOUNT_CHARACTER_SLOT_ID)
		if 0 != id:

			self.btnStart.Show()
			self.btnDelete.Show()
			self.btnRotationLeft.Show()
			self.btnRotationRight.Show()
			self.btnCreate.Hide()

			playTime=net.GetAccountCharacterSlotDataInteger(self.slot, net.ACCOUNT_CHARACTER_SLOT_PLAYTIME)
			level=net.GetAccountCharacterSlotDataInteger(self.slot, net.ACCOUNT_CHARACTER_SLOT_LEVEL)
			race=net.GetAccountCharacterSlotDataInteger(self.slot, net.ACCOUNT_CHARACTER_SLOT_RACE)
			valueHTH=net.GetAccountCharacterSlotDataInteger(self.slot, net.ACCOUNT_CHARACTER_SLOT_HTH)
			valueINT=net.GetAccountCharacterSlotDataInteger(self.slot, net.ACCOUNT_CHARACTER_SLOT_INT)
			valueSTR=net.GetAccountCharacterSlotDataInteger(self.slot, net.ACCOUNT_CHARACTER_SLOT_STR)
			valueDEX=net.GetAccountCharacterSlotDataInteger(self.slot, net.ACCOUNT_CHARACTER_SLOT_DEX)
			name=net.GetAccountCharacterSlotDataString(self.slot, net.ACCOUNT_CHARACTER_SLOT_NAME)
			guildID=net.GetAccountCharacterSlotDataInteger(self.slot, net.ACCOUNT_CHARACTER_SLOT_GUILD_ID)
			guildName=net.GetAccountCharacterSlotDataString(self.slot, net.ACCOUNT_CHARACTER_SLOT_GUILD_NAME)
			self.changeNameFlag=net.GetAccountCharacterSlotDataInteger(self.slot, net.ACCOUNT_CHARACTER_SLOT_CHANGE_NAME_FLAG)

			job = chr.RaceToJob(race)
			if job >= 0 and job < self.CHARACTER_TYPE_COUNT:
				self.destNameAlpha[job] = 1.0

			self.CharacterName.SetText(name)
			self.CharacterLevel.SetText(str(level))

			self.PlayTime.SetText(str(playTime))
			self.CharacterHTH.SetText(str(valueHTH))
			self.CharacterINT.SetText(str(valueINT))
			self.CharacterSTR.SetText(str(valueSTR))
			self.CharacterDEX.SetText(str(valueDEX))

			if guildName:
				self.GuildName.SetText(guildName)
			else:
				self.GuildName.SetText(localeInfo.SELECT_NOT_JOIN_GUILD)

			statesSummary = float(valueHTH + valueINT + valueSTR + valueDEX)
			if statesSummary > 0.0:
				self.destGauge =	[
										float(valueHTH) / statesSummary,
										float(valueINT) / statesSummary,
										float(valueSTR) / statesSummary,
										float(valueDEX) / statesSummary
									]

		else:

			self.InitCharacterBoard()

	def InitCharacterBoard(self):

		self.btnStart.Hide()
		self.btnDelete.Hide()
		self.btnRotationLeft.Hide()
		self.btnRotationRight.Hide()
		self.btnCreate.Show()

		self.CharacterName.SetText("")
		self.CharacterLevel.SetText("")
		self.PlayTime.SetText("")
		self.CharacterHTH.SetText("")
		self.CharacterINT.SetText("")
		self.CharacterSTR.SetText("")
		self.CharacterDEX.SetText("")
		self.GuildName.SetText(localeInfo.SELECT_NOT_JOIN_GUILD)

	## Event
	def OnKeyDown(self, key):

		if 1 == key:
			self.ExitSelect()
		if 2 == key:
			self.SelectSlot(0)
		if 3 == key:
			self.SelectSlot(1)
		if 4 == key:
			self.SelectSlot(2)
		if 5 == key:
			self.SelectSlot(3)
		if 6 == key:
			self.SelectSlot(4)

		if 28 == key:

			id = net.GetAccountCharacterSlotDataInteger(self.slot, net.ACCOUNT_CHARACTER_SLOT_ID)
			if 0 == id:
				self.CreateCharacter()

			else:
				self.StartGame()

		if 203 == key:
			self.slot = (self.GetSlotIndex() - 1 + self.SLOT_COUNT) % self.SLOT_COUNT
			self.SelectSlot(self.slot)
		if 205 == key:
			self.slot = (self.GetSlotIndex() + 1) % self.SLOT_COUNT
			self.SelectSlot(self.slot)

		return True

	def OnUpdate(self):
	
		background.Update(453.0,0.0,46.0,200.0)
		chr.Update()
		
		if self.btnRotationLeft.IsDown():
			self.Rotate("left")
		elif self.btnRotationRight.IsDown():
			self.Rotate("right")
		
		for i in xrange(self.SLOT_COUNT):
			if chr.IsWalking(i) != 1:
				if chr.HasInstance(i):
					chr.SelectInstance(i)
										
					if i != self.slot:
						chr.SetRotation(self.INSTANCE_POSITIONS[i]["rot"])
					else:
						if self.MainRotationHasChanges["index"] == i and self.MainRotationHasChanges["changes"] == True:
							return
						chr.SetRotation(self.INSTANCE_POSITIONS["dest"]["rot"])
					
				#chr.LookAtDestPixelPosition(self.INSTANCE_POSITIONS["look_dest"]["x"], self.INSTANCE_POSITIONS["look_dest"]["y"])
		

		for i in xrange(4):
			self.curGauge[i] += (self.destGauge[i] - self.curGauge[i]) / 10.0
			if abs(self.curGauge[i] - self.destGauge[i]) < 0.005:
				self.curGauge[i] = self.destGauge[i]
			self.GaugeList[i].SetPercentage(self.curGauge[i], 1.0)

		#######################################################
		if -1 != self.startIndex:

			## Temporary
			if app.GetTime() - self.startReservingTime > 0.0:
				if False == self.openLoadingFlag:
					chrSlot=self.stream.GetCharacterSlot()
					net.DirectEnter(chrSlot)
					self.openLoadingFlag = True

					playTime=net.GetAccountCharacterSlotDataInteger(self.slot, net.ACCOUNT_CHARACTER_SLOT_PLAYTIME)

					import player
					player.SetPlayTime(playTime)
					import chat
					chat.Clear()
			## Temporary
		#######################################################

	def EmptyFunc(self):
		pass

	def PopupMessage(self, msg, func=0):
		if not func:
			func=self.EmptyFunc

		self.stream.popupWindow.Close()
		self.stream.popupWindow.Open(msg, func, localeInfo.UI_OK)

	def OnPressExitKey(self):
		self.ExitSelect()
		return True
		
	def Rotate(self, dir):
		if not chr.IsWalking(self.slot):
			chr.SelectInstance(self.slot)
			if dir == "left":
				self.MainRotationHasChanges["rot"] -= 3.0
			elif dir == "right":
				self.MainRotationHasChanges["rot"] += 3.0
				
			chr.SetRotation(self.MainRotationHasChanges["rot"])
			
			if self.MainRotationHasChanges["changes"] == False:
				self.MainRotationHasChanges["changes"] = True

