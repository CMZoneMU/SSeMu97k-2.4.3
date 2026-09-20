-- =============================================================================
-- SSeMU MU Online Emulator - 97k
-- CalcCharacter.lua - Dynamic Character Calculation Script
-- Update 94 (2.5.2) -> 97k
-- =============================================================================

-- Function to calculate physical damage for Dark Wizard class
function DWPhysiDamageCalculate(aIndex)
    local Strength = GetObjectTotalStrength(aIndex)
    
    local PhysiDamageMinRight = Strength / 8
    local PhysiDamageMinLeft = Strength / 8
    local PhysiDamageMaxRight = Strength / 4
    local PhysiDamageMaxLeft = Strength / 4
    
    return PhysiDamageMinRight, PhysiDamageMinLeft, PhysiDamageMaxRight, PhysiDamageMaxLeft
end

-- Function to calculate magical damage for Dark Wizard class
function DWMagicDamageCalculate(aIndex)
    local Energy = GetObjectTotalEnergy(aIndex)
    
    local MagicDamageMin = Energy / 9
    local MagicDamageMax = Energy / 4
    
    return MagicDamageMin, MagicDamageMax
end

-- Function to calculate physical damage for Dark Knight class
function DKPhysiDamageCalculate(aIndex)
    local Strength = GetObjectTotalStrength(aIndex)
    
    local PhysiDamageMinRight = Strength / 6
    local PhysiDamageMinLeft = Strength / 6
    local PhysiDamageMaxRight = Strength / 4
    local PhysiDamageMaxLeft = Strength / 4
    
    return PhysiDamageMinRight, PhysiDamageMinLeft, PhysiDamageMaxRight, PhysiDamageMaxLeft
end

-- Function to calculate magical damage for Dark Knight class
function DKMagicDamageCalculate(aIndex)
    local Energy = GetObjectTotalEnergy(aIndex)
    
    local MagicDamageMin = Energy / 9
    local MagicDamageMax = Energy / 4
    
    return MagicDamageMin, MagicDamageMax
end

-- Function to calculate physical damage for Fairy Elf class
function FEPhysiDamageCalculate(aIndex, BowType)
    local Strength = GetObjectTotalStrength(aIndex)
    local Dexterity = GetObjectTotalDexterity(aIndex)
    
    if BowType == 0 then
        -- Without bow equipped
        local PhysiDamageMinRight = (Strength + Dexterity) / 7
        local PhysiDamageMinLeft = (Strength + Dexterity) / 7
        local PhysiDamageMaxRight = (Strength + Dexterity) / 4
        local PhysiDamageMaxLeft = (Strength + Dexterity) / 4
        return PhysiDamageMinRight, PhysiDamageMinLeft, PhysiDamageMaxRight, PhysiDamageMaxLeft
    else
        -- With bow equipped (BowType 1 or 2)
        local PhysiDamageMinRight = (Strength / 14) + (Dexterity / 7)
        local PhysiDamageMinLeft = (Strength / 14) + (Dexterity / 7)
        local PhysiDamageMaxRight = (Strength / 8) + (Dexterity / 4)
        local PhysiDamageMaxLeft = (Strength / 8) + (Dexterity / 4)
        return PhysiDamageMinRight, PhysiDamageMinLeft, PhysiDamageMaxRight, PhysiDamageMaxLeft
    end
end

-- Function to calculate magical damage for Fairy Elf class
function FEMagicDamageCalculate(aIndex)
    local Energy = GetObjectTotalEnergy(aIndex)
    
    local MagicDamageMin = Energy / 9
    local MagicDamageMax = Energy / 4
    
    return MagicDamageMin, MagicDamageMax
end

-- Function to calculate physical damage for Magic Gladiator class
function MGPhysiDamageCalculate(aIndex)
    local Strength = GetObjectTotalStrength(aIndex)
    local Energy = GetObjectTotalEnergy(aIndex)
    
    local PhysiDamageMinRight = (Strength / 6) + (Energy / 12)
    local PhysiDamageMinLeft = (Strength / 6) + (Energy / 12)
    local PhysiDamageMaxRight = (Strength / 4) + (Energy / 8)
    local PhysiDamageMaxLeft = (Strength / 4) + (Energy / 8)
    
    return PhysiDamageMinRight, PhysiDamageMinLeft, PhysiDamageMaxRight, PhysiDamageMaxLeft
end

-- Function to calculate magical damage for Magic Gladiator class
function MGMagicDamageCalculate(aIndex)
    local Energy = GetObjectTotalEnergy(aIndex)
    
    local MagicDamageMin = Energy / 9
    local MagicDamageMax = Energy / 4
    
    return MagicDamageMin, MagicDamageMax
end

-- Function to calculate right hand weapon damage
function PhysiDamageRight(ItemIndex, DamageMin, DamageMax)
    if ItemIndex >= GET_ITEM(5, 0) and ItemIndex < GET_ITEM(6, 0) then
        -- Staff damage halved
        local PhysiDamageMinRight = DamageMin / 2
        local PhysiDamageMaxRight = DamageMax / 2
        return PhysiDamageMinRight, PhysiDamageMaxRight
    else
        return DamageMin, DamageMax
    end
end

-- Function to calculate left hand weapon damage
function PhysiDamageLeft(ItemIndex, DamageMin, DamageMax)
    if ItemIndex >= GET_ITEM(5, 0) and ItemIndex < GET_ITEM(6, 0) then
        -- Staff damage halved
        local PhysiDamageMinLeft = DamageMin / 2
        local PhysiDamageMaxLeft = DamageMax / 2
        return PhysiDamageMinLeft, PhysiDamageMaxLeft
    else
        return DamageMin, DamageMax
    end
end

-- Function to calculate attack success rate
function CalcAttackSuccessRate(aIndex)
    local Class = GetObjectClass(aIndex)
    local Level = GetObjectTotalLevel(aIndex)
    local Strength = GetObjectTotalStrength(aIndex)
    local Dexterity = GetObjectTotalDexterity(aIndex)
    local AttackSuccessRate = 0
    
    if Class == 0 then -- CLASS_DW
        AttackSuccessRate = (Level * 5) + ((Dexterity * 3) / 2) + (Strength / 4)
    elseif Class == 1 then -- CLASS_DK
        AttackSuccessRate = (Level * 5) + ((Dexterity * 3) / 2) + (Strength / 4)
    elseif Class == 2 then -- CLASS_FE
        AttackSuccessRate = (Level * 5) + ((Dexterity * 3) / 2) + (Strength / 4)
    elseif Class == 3 then -- CLASS_MG
        AttackSuccessRate = (Level * 5) + ((Dexterity * 3) / 2) + (Strength / 4)
    end
    
    return AttackSuccessRate
end

-- Function to calculate attack speed
function CalcAttackSpeed(aIndex)
    local Class = GetObjectClass(aIndex)
    local Dexterity = GetObjectTotalDexterity(aIndex)
    local PhysiSpeed = 0
    local MagicSpeed = 0
    
    if Class == 0 then -- CLASS_DW
        PhysiSpeed = Dexterity / 20
        MagicSpeed = Dexterity / 10
    elseif Class == 1 then -- CLASS_DK
        PhysiSpeed = Dexterity / 15
        MagicSpeed = Dexterity / 20
    elseif Class == 2 then -- CLASS_FE
        PhysiSpeed = Dexterity / 50
        MagicSpeed = Dexterity / 50
    elseif Class == 3 then -- CLASS_MG
        PhysiSpeed = Dexterity / 15
        MagicSpeed = Dexterity / 20
    end
    
    return PhysiSpeed, MagicSpeed
end

-- Function to calculate defense success rate
function CalcDefenseSuccessRate(aIndex)
    local Class = GetObjectClass(aIndex)
    local Dexterity = GetObjectTotalDexterity(aIndex)
    local DefenseSuccessRate = 0
    
    if Class == 0 then -- CLASS_DW
        DefenseSuccessRate = Dexterity / 3
    elseif Class == 1 then -- CLASS_DK
        DefenseSuccessRate = Dexterity / 3
    elseif Class == 2 then -- CLASS_FE
        DefenseSuccessRate = Dexterity / 4
    elseif Class == 3 then -- CLASS_MG
        DefenseSuccessRate = Dexterity / 3
    end
    
    return DefenseSuccessRate
end

-- Function to calculate defense
function CalcDefense(aIndex)
    local Class = GetObjectClass(aIndex)
    local Dexterity = GetObjectTotalDexterity(aIndex)
    local Defense = 0
    
    if Class == 0 then -- CLASS_DW
        Defense = Dexterity / 4
    elseif Class == 1 then -- CLASS_DK
        Defense = Dexterity / 3
    elseif Class == 2 then -- CLASS_FE
        Defense = Dexterity / 10
    elseif Class == 3 then -- CLASS_MG
        Defense = Dexterity / 4
    end
    
    return Defense
end

-- Function to calculate bonus defense based on set item level
function CalcBonusDefense(aIndex, BonusLevel)
    local Defense = GetObjectDefense(aIndex)
    local BonusDefense = 0
    
    if BonusLevel == 0 then -- Set +10
        BonusDefense = Defense + ((Defense * 5) / 100)
    elseif BonusLevel == 1 then -- Set +11
        BonusDefense = Defense + ((Defense * 10) / 100)
    elseif BonusLevel == 2 then -- Set +12
        BonusDefense = Defense + ((Defense * 15) / 100)
    elseif BonusLevel == 3 then -- Set +13
        BonusDefense = Defense + ((Defense * 20) / 100)
    elseif BonusLevel == 4 then -- Set +14
        BonusDefense = Defense + ((Defense * 25) / 100)
    elseif BonusLevel == 5 then -- Set +15
        BonusDefense = Defense + ((Defense * 30) / 100)
    end
    
    return BonusDefense
end