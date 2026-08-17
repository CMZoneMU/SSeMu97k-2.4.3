--############################################################################
-- SSeMU MU Online Emulator
-- https://www.ssemu.com.ar
-- https://www.facebook.com/ssemuemulator
-- https://www.youtube.com/@ssemu
-- ---------------------------------------------------------------------------
-- © 2024 SetecSoft Development
-- This file is part of the SSeMU MuOnline Server files.
--############################################################################

-- Function to calculate physical damage for Dark Wizard class
function DWPhysiDamageCalculate(aIndex)
    
    local Strength = GetObjectTotalStrength(aIndex)
    
    -- Calculate minimum and maximum physical damage for right and left hand
    local PhysiDamageMinRight = Strength / 8 -- Minimum Right Hand Physical Damage
    local PhysiDamageMinLeft = Strength / 8 -- Minimum Left Hand Physical Damage
    local PhysiDamageMaxRight = Strength / 4 -- Maximum Right Hand Physical Damage
    local PhysiDamageMaxLeft = Strength / 4 -- Maximum Left Hand Physical Damage
	
    return PhysiDamageMinRight, PhysiDamageMinLeft, PhysiDamageMaxRight, PhysiDamageMaxLeft
    
end

-- Function to calculate magical damage for Dark Wizard class
function DWMagicDamageCalculate(aIndex)
    
    local Energy = GetObjectTotalEnergy(aIndex)
    
    -- Calculate minimum and maximum magical damage
    local MagicDamageMin = Energy / 9 -- Minimum Magical Damage
    local MagicDamageMax = Energy / 4 -- Maximum Magical Damage
	
    return MagicDamageMin, MagicDamageMax
    
end

-- Function to calculate physical damage for Dark Knight class
function DKPhysiDamageCalculate(aIndex)
    
    local Strength = GetObjectTotalStrength(aIndex)
    
    -- Calculate minimum and maximum physical damage for right and left hand
    local PhysiDamageMinRight = Strength / 6 -- Minimum Right Hand Physical Damage
    local PhysiDamageMinLeft = Strength / 6 -- Minimum Left Hand Physical Damage
    local PhysiDamageMaxRight = Strength / 4 -- Maximum Right Hand Physical Damage
    local PhysiDamageMaxLeft = Strength / 4 -- Maximum Left Hand Physical Damage
	
    return PhysiDamageMinRight, PhysiDamageMinLeft, PhysiDamageMaxRight, PhysiDamageMaxLeft
    
end

-- Function to calculate magical damage for Dark Knight class
function DKMagicDamageCalculate(aIndex)
    
    local Energy = GetObjectTotalEnergy(aIndex)
    
    -- Calculate minimum and maximum magical damage
    local MagicDamageMin = Energy / 9 -- Minimum Magical Damage
    local MagicDamageMax = Energy / 4 -- Maximum Magical Damage
	
    return MagicDamageMin, MagicDamageMax
    
end

-- Function to calculate physical damage for Fairy Elf class
function FEPhysiDamageCalculate(aIndex, BowType)
    
    local Strength = GetObjectTotalStrength(aIndex)
    local Dexterity = GetObjectTotalDexterity(aIndex)
    
    if BowType == 0 then
    
        -- Calculate minimum and maximum physical damage without bow equipped
        local PhysiDamageMinRight = (Strength + Dexterity) / 7 -- Minimum Right Hand Physical Damage Without Bow
        local PhysiDamageMinLeft = (Strength + Dexterity) / 7 -- Minimum Left Hand Physical Damage Without Bow
        local PhysiDamageMaxRight = (Strength + Dexterity) / 4 -- Maximum Right Hand Physical Damage Without Bow
        local PhysiDamageMaxLeft = (Strength + Dexterity) / 4 -- Maximum Left Hand Physical Damage Without Bow
		
        return PhysiDamageMinRight, PhysiDamageMinLeft, PhysiDamageMaxRight, PhysiDamageMaxLeft
    
    else
    
        -- Calculate minimum and maximum physical damage with bow equipped
        local PhysiDamageMinRight = (Strength / 14) + (Dexterity / 7) -- Minimum Right Hand Physical Damage With Bow
        local PhysiDamageMinLeft = (Strength / 14) + (Dexterity / 7) -- Minimum Left Hand Physical Damage With Bow
        local PhysiDamageMaxRight = (Strength / 8) + (Dexterity / 4) -- Maximum Right Hand Physical Damage With Bow
        local PhysiDamageMaxLeft = (Strength / 8) + (Dexterity / 4) -- Maximum Left Hand Physical Damage With Bow
    
        return PhysiDamageMinRight, PhysiDamageMinLeft, PhysiDamageMaxRight, PhysiDamageMaxLeft
        
    end
    
end

-- Function to calculate magical damage for Fairy Elf class
function FEMagicDamageCalculate(aIndex)
    
    local Energy = GetObjectTotalEnergy(aIndex)
    
    -- Calculate minimum and maximum magical damage
    local MagicDamageMin = Energy / 9 -- Minimum Magical Damage
    local MagicDamageMax = Energy / 4 -- Maximum Magical Damage
	
    return MagicDamageMin, MagicDamageMax
    
end

-- Function to calculate physical damage for Magic Gladiator class
function MGPhysiDamageCalculate(aIndex)
    
    local Strength = GetObjectTotalStrength(aIndex)
    local Energy = GetObjectTotalEnergy(aIndex)
    
    -- Calculate minimum and maximum physical damage for right and left hand
    local PhysiDamageMinRight = (Strength / 6) + (Energy / 12) -- Minimum Right Hand Physical Damage
    local PhysiDamageMinLeft = (Strength / 6) + (Energy / 12) -- Minimum Left Hand Physical Damage
    local PhysiDamageMaxRight = (Strength / 4) + (Energy / 8) -- Maximum Right Hand Physical Damage
    local PhysiDamageMaxLeft = (Strength / 4) + (Energy / 8) -- Maximum Left Hand Physical Damage
	
    return PhysiDamageMinRight, PhysiDamageMinLeft, PhysiDamageMaxRight, PhysiDamageMaxLeft
    
end

-- Function to calculate magical damage for Magic Gladiator class
function MGMagicDamageCalculate(aIndex)
    
    local Energy = GetObjectTotalEnergy(aIndex)
    
    -- Calculate minimum and maximum magical damage
    local MagicDamageMin = Energy / 9 -- Minimum Magical Damage
    local MagicDamageMax = Energy / 4 -- Maximum Magical Damage
	
    return MagicDamageMin, MagicDamageMax
    
end

-- Function to calculate right hand physical damage
function PhysiDamageRight(ItemIndex, DamageMin, DamageMax)
    
    if ItemIndex >= GET_ITEM(5,0) and ItemIndex < GET_ITEM(6,0) then
    
        -- Reduce damage for certain item types (Staffs)
        local PhysiDamageMinRight = DamageMin / 2 -- Halved Minimum Right Hand Physical Damage
        local PhysiDamageMaxRight = DamageMax / 2 -- Halved Maximum Right Hand Physical Damage
		
        return PhysiDamageMinRight, PhysiDamageMaxRight
    else
    
        local PhysiDamageMinRight = DamageMin -- Full Minimum Right Hand Physical Damage
        local PhysiDamageMaxRight = DamageMax -- Full Maximum Right Hand Physical Damage
		
        return PhysiDamageMinRight, PhysiDamageMaxRight
    end
    
end

-- Function to calculate left hand physical damage
function PhysiDamageLeft(ItemIndex, DamageMin, DamageMax)
    
    if ItemIndex >= GET_ITEM(5,0) and ItemIndex < GET_ITEM(6,0) then
        
        -- Reduce damage for certain item types (Staffs)
        local PhysiDamageMinLeft = DamageMin / 2 -- Halved Minimum Left Hand Physical Damage
        local PhysiDamageMaxLeft = DamageMax / 2 -- Halved Maximum Left Hand Physical Damage
		
        return PhysiDamageMinLeft, PhysiDamageMaxLeft
    
    else
    
        local PhysiDamageMinLeft = DamageMin -- Full Minimum Left Hand Physical Damage
        local PhysiDamageMaxLeft = DamageMax -- Full Maximum Left Hand Physical Damage
		
        return PhysiDamageMinLeft, PhysiDamageMaxLeft
    
    end
    
end

-- Function to calculate special physical damage (applies only to warrior classes when equipping two weapons)
function PhysiDamageSpecial(aIndex, DamageMinRight, DamageMaxRight, DamageMinLeft, DamageMaxLeft)
    
    -- Reduce damage by 55%
    local PhysiDamageMinRight = (DamageMinRight * 55) / 100 -- 55% of Minimum Right Hand Physical Damage
    local PhysiDamageMaxRight = (DamageMaxRight * 55) / 100 -- 55% of Maximum Right Hand Physical Damage
    local PhysiDamageMinLeft = (DamageMinLeft * 55) / 100 -- 55% of Minimum Left Hand Physical Damage
    local PhysiDamageMaxLeft = (DamageMaxLeft * 55) / 100 -- 55% of Maximum Left Hand Physical Damage
	
    return PhysiDamageMinRight, PhysiDamageMaxRight, PhysiDamageMinLeft, PhysiDamageMaxLeft
    
end

-- Function to calculate the attack success rate for different classes
function CalcAttackSuccessRate(aIndex)

    local Class = GetObjectClass(aIndex)
    local Level = GetObjectLevel(aIndex)
    local Strength = GetObjectTotalStrength(aIndex)
    local Dexterity = GetObjectTotalDexterity(aIndex)
    local AttackSuccessRate = 0
    
    if Class == 0 then -- CLASS_DW (Dark Wizard)
        
        AttackSuccessRate = (Level * 5) + ((Dexterity * 3) / 2) + (Strength / 4)
        
    elseif Class == 1 then -- CLASS_DK (Dark Knight)
            
        AttackSuccessRate = (Level * 5) + ((Dexterity * 3) / 2) + (Strength / 4)
        
    elseif Class == 2 then -- CLASS_FE (Fairy Elf)
            
        AttackSuccessRate = (Level * 5) + ((Dexterity * 3) / 2) + (Strength / 4)
        
    elseif Class == 3 then -- CLASS_MG (Magic Gladiator)
            
        AttackSuccessRate = (Level * 5) + ((Dexterity * 3) / 2) + (Strength / 4)
        
    end
    
    return AttackSuccessRate

end

-- Function to calculate the defense success rate for different classes
function CalcDefenseSuccessRate(aIndex)

    local Class = GetObjectClass(aIndex)
    local Dexterity = GetObjectTotalDexterity(aIndex)
    local DefenseSuccessRate = 0
    
    if Class == 0 then -- CLASS_DW (Dark Wizard)
        
        DefenseSuccessRate = Dexterity / 3
        
    elseif Class == 1 then -- CLASS_DK (Dark Knight)
            
        DefenseSuccessRate = Dexterity / 3
        
    elseif Class == 2 then -- CLASS_FE (Fairy Elf)
            
        DefenseSuccessRate = Dexterity / 4
        
    elseif Class == 3 then -- CLASS_MG (Magic Gladiator)
            
        DefenseSuccessRate = Dexterity / 3
        
    end
    
    return DefenseSuccessRate

end

-- Function to calculate the defense for different classes
function CalcDefense(aIndex)

    local Class = GetObjectClass(aIndex)
    local Dexterity = GetObjectTotalDexterity(aIndex)
    local Defense = 0
    
    if Class == 0 then -- CLASS_DW (Dark Wizard)
        
        Defense = Dexterity / 4
        
    elseif Class == 1 then -- CLASS_DK (Dark Knight)
            
        Defense = Dexterity / 3
        
    elseif Class == 2 then -- CLASS_FE (Fairy Elf)
            
        Defense = Dexterity / 10
        
    elseif Class == 3 then -- CLASS_MG (Magic Gladiator)
            
        Defense = Dexterity / 4
        
    end
    
    return Defense

end

-- Function to calculate bonus defense based on the bonus level
function CalcBonusDefense(aIndex, BonusLevel)

    local Defense = GetObjectDefense(aIndex)
    local BonusDefense = 0
    
    if BonusLevel == 0 then -- Bonus Set (+10)
        
        BonusDefense = Defense + ((Defense * 5) / 100)
        
    elseif BonusLevel == 1 then -- Bonus Set (+11)
            
        BonusDefense = Defense + ((Defense * 10) / 100)
        
    end
	
    return BonusDefense

end

-- Function to calculate the attack speed for different classes
function CalcAttackSpeed(aIndex)

    local Class = GetObjectClass(aIndex)
    local Dexterity = GetObjectTotalDexterity(aIndex)
    local PhysiSpeed = 0
    local MagicSpeed = 0
    
    if Class == 0 then -- CLASS_DW (Dark Wizard)
        
        PhysiSpeed = Dexterity / 20
        MagicSpeed = Dexterity / 10
        
    elseif Class == 1 then -- CLASS_DK (Dark Knight)
            
        PhysiSpeed = Dexterity / 15
        MagicSpeed = Dexterity / 20
        
    elseif Class == 2 then -- CLASS_FE (Fairy Elf)
            
        PhysiSpeed = Dexterity / 50
        MagicSpeed = Dexterity / 50
        
    elseif Class == 3 then -- CLASS_MG (Magic Gladiator)
            
        PhysiSpeed = Dexterity / 15
        MagicSpeed = Dexterity / 20
        
    end
    
    return PhysiSpeed, MagicSpeed

end