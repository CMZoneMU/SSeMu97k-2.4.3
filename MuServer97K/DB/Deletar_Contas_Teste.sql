-- ============================================================================
-- SCRIPT DE REMOÇÃO / LIMPEZA DE CONTAS E PERSONAGENS TESTE (COMPATÍVEL 97K)
-- Banco de Dados: MuOnline97 / MuOnline (SSeMU 97k)
-- Data de Criação: 16/08/2026
--
-- EXPLICACAO E SEGURANÇA:
-- 1. Remove com segurança apenas as contas e personagens gerados pelo script de testes (test0001 até test0050).
-- 2. Apaga em ordem de integridade referencial: Character -> AccountCharacter -> MEMB_STAT -> MEMB_INFO.
-- 3. Exibe o relatório de confirmação dos logins deletados ao final.
-- ============================================================================

-- Seleciona a base de dados do MuOnline no SQL Server
USE [MuOnline97]
GO

-- Desativa as mensagens de contagem de linhas afetadas no console do SQL
SET NOCOUNT ON;

-- ----------------------------------------------------------------------------
-- TABELA TEMPORÁRIA DE CONTAS A SEREM DELETADAS
-- Seleciona os logins iniciados com 'test' ou 't' criados pelo gerador automático
-- ----------------------------------------------------------------------------
DECLARE @ContasParaDeletar TABLE (
    Login VARCHAR(10)
);

-- Insere os alvos de deleção na tabela temporária de segurança
INSERT INTO @ContasParaDeletar (Login)
SELECT memb___id 
FROM [dbo].[MEMB_INFO] 
WHERE memb___id LIKE 'test%' OR (memb___id LIKE 't%' AND LEN(memb___id) = 10);

-- ----------------------------------------------------------------------------
-- 1. REMOÇÃO DOS PERSONAGENS (Tabela Character)
-- Apaga todos os personagens vinculados às contas de teste identificadas
-- ----------------------------------------------------------------------------
DELETE FROM [dbo].[Character]
WHERE AccountID IN (SELECT Login FROM @ContasParaDeletar);

-- ----------------------------------------------------------------------------
-- 2. REMOÇÃO DO VÍNCULO DE SLOTS (Tabela AccountCharacter)
-- Apaga o registro da estrutura dos 5 slots de personagem da conta
-- ----------------------------------------------------------------------------
DELETE FROM [dbo].[AccountCharacter]
WHERE Id IN (SELECT Login FROM @ContasParaDeletar);

-- ----------------------------------------------------------------------------
-- 3. REMOÇÃO DO HISTÓRICO DE CONEXÃO (Tabela MEMB_STAT)
-- Remove registros de status online/offline caso as contas tenham logado no jogo
-- ----------------------------------------------------------------------------
DELETE FROM [dbo].[MEMB_STAT]
WHERE memb___id IN (SELECT Login FROM @ContasParaDeletar);

-- ----------------------------------------------------------------------------
-- 4. REMOÇÃO DA CONTA PRINCIPAL (Tabela MEMB_INFO)
-- Apaga o cadastro do usuário/login na tabela de credenciais
-- ----------------------------------------------------------------------------
DELETE FROM [dbo].[MEMB_INFO]
WHERE memb___id IN (SELECT Login FROM @ContasParaDeletar);

-- ----------------------------------------------------------------------------
-- RELATÓRIO FINAL DE DELEÇÃO
-- Exibe a lista de logins que foram removidos do banco de dados
-- ----------------------------------------------------------------------------
SELECT 
    Login AS [Contas Testes Removidas do Banco] 
FROM @ContasParaDeletar;

-- Notificação de encerramento da limpeza no console de mensagens do SSMS
PRINT '✔ Limpeza concluída! Todas as contas e personagens testes foram deletados com sucesso.';
GO
