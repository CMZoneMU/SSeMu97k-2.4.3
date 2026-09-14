-- ============================================================================
-- SSeMU 97K - Update 93 (v2.5.1) Database Migration Script
-- Tabelas e Colunas para os novos sistemas:
-- 1. [Character].[IsNewChar] (DefaultClassFreebies)
-- 2. [GiftData] (CustomGift /gift)
-- 3. [MonsterKillCount] (CustomMonsterKillCount)
-- ============================================================================

USE [MuOnline]
GO

-- 1. Adiciona coluna IsNewChar na tabela Character (para controle de presentes iniciais de novos personagens)
IF NOT EXISTS (SELECT * FROM sys.columns WHERE object_id = OBJECT_ID(N'[dbo].[Character]') AND name = 'IsNewChar')
BEGIN
    ALTER TABLE [dbo].[Character] ADD [IsNewChar] [int] NOT NULL CONSTRAINT [DF_Character_IsNewChar] DEFAULT ((1))
    PRINT '[Update 2.5.1] Coluna IsNewChar adicionada com sucesso na tabela Character.'
END
ELSE
BEGIN
    PRINT '[Update 2.5.1] Coluna IsNewChar ja existe na tabela Character.'
END
GO

-- 2. Cria tabela GiftData (para persistencia do resgate de presentes por personagem via /gift)
IF NOT EXISTS (SELECT * FROM sys.objects WHERE object_id = OBJECT_ID(N'[dbo].[GiftData]') AND type in (N'U'))
BEGIN
    CREATE TABLE [dbo].[GiftData](
        [Name] [varchar](10) NOT NULL,
        [Index] [int] NOT NULL,
        [Count] [int] NOT NULL CONSTRAINT [DF_GiftData_Count] DEFAULT ((0)),
        CONSTRAINT [PK_GiftData] PRIMARY KEY CLUSTERED 
        (
            [Name] ASC,
            [Index] ASC
        ) ON [PRIMARY]
    ) ON [PRIMARY]
    PRINT '[Update 2.5.1] Tabela GiftData criada com sucesso.'
END
ELSE
BEGIN
    PRINT '[Update 2.5.1] Tabela GiftData ja existe.'
END
GO

-- 3. Cria tabela MonsterKillCount (para persistencia da contagem de monstros abatidos por personagem)
IF NOT EXISTS (SELECT * FROM sys.objects WHERE object_id = OBJECT_ID(N'[dbo].[MonsterKillCount]') AND type in (N'U'))
BEGIN
    CREATE TABLE [dbo].[MonsterKillCount](
        [Name] [varchar](10) NOT NULL,
        [MonsterClass] [int] NOT NULL,
        [MonsterKillCount] [int] NOT NULL CONSTRAINT [DF_MonsterKillCount_Count] DEFAULT ((0)),
        CONSTRAINT [PK_MonsterKillCount] PRIMARY KEY CLUSTERED 
        (
            [Name] ASC,
            [MonsterClass] ASC
        ) ON [PRIMARY]
    ) ON [PRIMARY]
    PRINT '[Update 2.5.1] Tabela MonsterKillCount criada com sucesso.'
END
ELSE
BEGIN
    PRINT '[Update 2.5.1] Tabela MonsterKillCount ja existe.'
END
GO
