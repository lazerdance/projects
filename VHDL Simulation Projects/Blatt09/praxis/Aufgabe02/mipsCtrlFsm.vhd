library ieee;
use ieee.std_logic_1164.all;

library ROrgPrSimLib;
use ROrgPrSimLib.proc_config.mips_ctrl_state_type;
use ROrgPrSimLib.mipsISA.all;

entity mipsCtrlFsm is
port(clk : in std_logic;
	 rst : in std_logic;
	 op : in std_logic_vector(5 downto 0);
	 regDst : out std_logic;
	 memRead : out std_logic;
	 memToReg : out std_logic;
	 aluOp : out std_logic_vector(1 downto 0);
	 memWrite : out std_logic;
	 regWrite : out std_logic;
	 aluSrcA : out std_logic;
	 aluSrcB : out std_logic_vector(1 downto 0);
	 pcSrc : out std_logic_vector(1 downto 0);
	 irWrite : out std_logic;
	 IorD : out std_logic;
	 pcWrite : out std_logic;
	 pcWriteCond : out std_logic;
	 
	 -- debug port
	 mipsCtrlState_debug : out mips_ctrl_state_type);
end mipsCtrlFsm;

architecture behavioral of mipsCtrlFsm is
	
	signal state_reg, state_next : mips_ctrl_state_type;
	
begin

	-- Beschreibung der MIPS Control FSM hier einfügen
	state:process(clk, rst)
	begin
        -- ändern von state_reg
        if (rst = '1') then
            state_reg <= INSTR_FETCH;
        elsif (clk'EVENT and clk = '1') then
            state_reg <= state_next;
        end if;
        
    end process;
    
	output:process(state_reg)
	begin
        -- decode von state
        case state_reg is
            when INSTR_FETCH =>
                memRead <= '1';
                aluSrcA <= '0';
                IorD <= '0';
                irWrite <= '1';
                aluSrcB <= "01";
                aluOp <= "00";
                pcWrite <= '1';
                pcSrc <= "00";
                --reset to default:
                regDst <= '0';
                regWrite <= '0';
                memToReg <= '0';
                memWrite <= '0';
                pcWriteCond <= '0';
            when INSTR_DECODE =>
                aluSrcA <= '0';
                aluSrcB <= "11";
                aluOp <= "00";
                --reset to default:
                memRead <= '0';
                IorD <= '0';
                irWrite <= '0';
                pcWrite <= '0';
                pcSrc <= "00";
            when MEM_ADDR_CALC =>
                aluSrcA <= '1';
                aluSrcB <= "10";
                aluOp <= "00";
            when MEM_READ =>
                memRead <= '1';
                IorD <= '1';
                --reset to default:
                aluSrcA <= '0';
                aluSrcB <= "00";
                aluOp <= "00";
            when MEM_READ_COMPL =>
                regDst <= '0';
                regWrite <= '1';
                memToReg <= '1';
                --reset to default:
                memRead <= '0';
                IorD <= '0';
            when MEM_WRITE =>
                memWrite <= '1';
                IorD <= '1';
                --reset to default:
                aluSrcA <= '0';
                aluSrcB <= "00";
                aluOp <= "00";
            when EXECUTION =>
                aluSrcA <= '1';
                aluSrcB <=  "00";
                aluOp <= "10";
            when RTYPE_COMPL =>
                regDst <= '1';
                regWrite <= '1';
                memToReg<= '0';
                --reset to default:
                aluSrcA <= '0';
                aluSrcB <= "00";
                aluOp <= "00";
            when BRANCH_COMPL =>
                aluSrcA <= '1';
                aluSrcB <= "00";
                aluOp <= "01";
                pcWriteCond <= '1';
                pcSrc <= "01";
            when JUMP_COMPL =>
                pcWrite <= '1';
                pcSrc <= "10";
                --reset to default:
                aluSrcA <= '0';
                aluSrcB <= "00";
                aluOp <= "00";
        end case;
    end process;
    
	transition:process(state_reg, op)
	begin
        -- ändern von state
        case state_reg is
            when INSTR_FETCH =>
                state_next <= INSTR_DECODE;
            when INSTR_DECODE =>
                case op is
                    when LW_OPCODE =>
                        state_next <= MEM_ADDR_CALC;
                    when SW_OPCODE =>
                        state_next <= MEM_ADDR_CALC;
                    when BEQ_OPCODE =>
                        state_next <= BRANCH_COMPL;
                    when J_OPCODE =>
                        state_next <= JUMP_COMPL;
                    when R_FORMAT_OPCODE =>--TODO RTYPE (?)
                        state_next <= EXECUTION;
                    when others =>
                        state_next <= INSTR_FETCH;
                end case;
            when EXECUTION =>
                state_next <= RTYPE_COMPL;
            when MEM_ADDR_CALC =>
                case op is
                    when LW_OPCODE =>
                        state_next <= MEM_READ;
                    when others =>
                        state_next <= MEM_WRITE;
                end case;
            when MEM_READ =>
                state_next <= MEM_READ_COMPL;
            when others =>
                state_next <= INSTR_FETCH;
        end case;
    end process;
	
	
	
	mipsCtrlState_debug <= state_reg;
	
end behavioral;
