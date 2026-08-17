--############################################################################
-- SSeMU MU Online Emulator
-- https://www.ssemu.com.ar
-- https://www.facebook.com/ssemuemulator
-- https://www.youtube.com/@ssemu
-- ---------------------------------------------------------------------------
-- © 2023 SetecSoft Development
-- This file is part of the SSeMU MuOnline Server files.
--############################################################################

-- ===========================================================================
BridgeFunctionAttach('OnCharacterEntry','WelcomeMessage_OnCharacterEntry')
BridgeFunctionAttach('OnCharacterClose','WelcomeMessage_OnCharacterClose')
-- ===========================================================================

function WelcomeMessage_OnCharacterEntry(aIndex)
	
	local UserName = GetObjectName(aIndex)
	
	local UserAccountLevel = GetObjectAccountLevel(aIndex)
	
	local UserAccountExpireDate = GetObjectAccountExpireDate(aIndex)

	NoticeSend(aIndex,0,string.format(MessageGet(154),UserName))

	if UserAccountLevel == 0 then 
		
		NoticeSend(aIndex,1,string.format(MessageGet(155),UserAccountExpireDate))
		
	elseif UserAccountLevel == 1 then
		
		NoticeSend(aIndex,1,string.format(MessageGet(156),UserAccountExpireDate))
		
	elseif UserAccountLevel == 2 then
		
		NoticeSend(aIndex,1,string.format(MessageGet(157),UserAccountExpireDate))
		
	elseif UserAccountLevel == 3 then
		
		NoticeSend(aIndex,1,string.format(MessageGet(158),UserAccountExpireDate))
		
	end
	
	local GameMasterLevel = CommandCheckGameMasterLevel(aIndex,8)
	
	if GameMasterLevel == 1 then
		
		NoticeSendToAll(0,string.format("Gamemaster %s is now connected!",UserName))
		
	end
	
	MessageSend(aIndex,2,2,"Welcome to SSeMU Emulator")
	
	MessageSend(aIndex,2,6,"https://www.ssemu.com.ar")
	
	MessageSend(aIndex,2,6,"https://www.facebook.com/ssemuemulator")
	
	MessageSend(aIndex,2,6,"https://www.youtube.com/@ssemu")
	
end

function WelcomeMessage_OnCharacterClose(aIndex)

	local UserName = GetObjectName(aIndex)
	
	local GameMasterLevel = CommandCheckGameMasterLevel(aIndex,8)
	
	if GameMasterLevel == 1 then
		
		NoticeSendToAll(0,string.format("Gamemaster %s disconnected!",UserName))
		
	end
end