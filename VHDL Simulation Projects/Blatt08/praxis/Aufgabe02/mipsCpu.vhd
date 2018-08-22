library ieee;
use ieee.std_logic_1164.all;
use ieee.numeric_std.all;

library ROrgPrSimLib;
use ROrgPrSimLib.proc_config.all;
use ROrgPrSimLib.flashROM;
use ROrgPrSimLib.flashRAM;

library work;
use work.all;

entity mipsCpu is
	generic(PROG_FILE_NAME : string;
			DATA_FILE_NAME : string);
	port(clk : in std_logic;
		 rst : in std_logic;
		 
		 -- instruction insertion ports
		 testMode_debug : in std_logic;
		 testInstruction_debug : in std_logic_vector(31 downto 0);
		 
		 -- ram access ports
		 ramInsertMode_debug : in std_logic;
		 ramWriteEn_debug : in std_logic;
		 ramWriteAddr_debug : in std_logic_vector(LOG2_NUM_RAM_ELEMENTS - 1 downto 0);
		 ramWriteData_debug : in std_logic_vector(RAM_ELEMENT_WIDTH - 1 downto 0);
		 ramElements_debug : out ram_elements_type;
		 
		 -- register file access port
		 registers_debug : out reg_vector_type;
		 
		 -- intermediate result ports
		 pc_next_debug : out std_logic_vector(PC_WIDTH - 1 downto 0);
		 pc7SegDigits_debug : out pc_7seg_digits_type
		 );
end mipsCpu;

architecture structural of mipsCpu is
signal readInstruction, instruction, shift : std_logic_vector(31 downto 0) := (others => '0');
signal regDst, branch, memRead, memToReg, memWrite, aluSrc, regWrite, invClk, aluZero, zeroAndBranch : std_logic := '0';
signal aluOp : std_logic_vector(1 downto 0) := (others => '0');
signal writeAddr : std_logic_vector(4 downto 0) := (others => '0');
signal reg1, reg2, aluResult, regData, aluIn2 : std_logic_vector(REG_WIDTH-1 downto 0) := (others => '0');
signal aluCode : std_logic_vector(3 downto 0) := (others => '0');
signal ramRead : std_logic_vector(RAM_ELEMENT_WIDTH-1 downto 0) := (others => '0');
signal PC_now, PC_next : std_logic_vector(PC_WIDTH-1 downto 0) := "00000000000000000000000000000000";
signal siExt : signed(31 downto 0) := (others => '0');
signal RAM_writeEn : std_logic := '0';
signal RAM_address : std_logic_vector(9 downto 0) := (others => '0');
signal RAM_data : std_logic_vector(RAM_ELEMENT_WIDTH-1 downto 0) := (others => '0');
begin

	-- Beschreibung der MIPS-CPU hier ergänzen
	CONTROL: entity work.mipsCtrl(structural)
        port map(op => instruction(31 downto 26),
            regDst => regDst,
            branch => branch,
            memRead => memRead,
            memToReg => memToReg,
            aluOp => aluOp,
            memWrite => memWrite,
            aluSrc => aluSrc,
            regWrite => regWrite);
            
    SIGN_EXTEND: entity work.signExtend(behavioral)
        generic map(INPUT_WIDTH => 16,
                    OUTPUT_WIDTH => 32)
        port map(number => signed(instruction(15 downto 0)),
                signExtNumber => siExt);
                
    LEFT_SHIFT: entity work.leftShifter(behavioral)
        generic map(WIDTH => 32,
                    SHIFT_AMOUNT => 2)
        port map(number => std_logic_vector(siExt),
                shiftedNumber => shift);
                
    REGISTRY: entity work.regFile(structural)
        generic map(NUM_REGS => NUM_REGS,
                    LOG2_NUM_REGS => LOG2_NUM_REGS,
                    REG_WIDTH => REG_WIDTH)
        port map(clk => clk,
                rst => rst,
                readAddr1 => instruction(25 downto 21),
                readData1 => reg1,
                readAddr2 => instruction(20 downto 16),
                readData2 => reg2,
                writeEn => regWrite,
                writeAddr => writeAddr,
                writeData => regData,
                reg_vect_debug => registers_debug);
                
	ALU_CTRL: entity work.aluCtrl(behavioral)
        port map(aluOp => aluOp,
                f => instruction(5 downto 0),
                operation => aluCode);
                
    ALU: entity work.csAlu(behavioral)
        generic map(WIDTH => REG_WIDTH)
        port map(ctrl => aluCode,
                a => reg1,
                b => aluIn2,
                result => aluResult,
                overflow => open,
                zero => aluZero);
                
    zeroAndBranch <= aluZero and branch;
    pcAndMux: process(PC_now, shift, zeroAndBranch, clk, PC_next, testMode_debug, ramRead, aluResult, siExt, reg2, testInstruction_debug, readInstruction, ramWriteAddr_debug, ramWriteData_debug, ramWriteEn_debug, memWrite)
    begin
        if (clk'EVENT and clk = '1') then
            if(rst = '1') then
                PC_now <= (others => '0');
            elsif(testMode_debug = '0') then
                PC_now <= PC_next;
            end if;
        end if;
        case regDst is
            when '1' => writeAddr <= instruction(15 downto 11);
            when others => writeAddr <= instruction(20 downto 16);
        end case;
        case memToReg is
            when '1' => regData <= ramRead;
            when others => regData <= aluResult;
        end case;
        case aluSrc is
            when '1' => aluIn2 <= std_logic_vector(siExt);
            when others => aluIn2 <= reg2;
        end case;
        case zeroAndBranch is
            when '1' => PC_next <= std_logic_vector(signed(PC_now)+signed(shift));
            when others => PC_next <= std_logic_vector(signed(PC_now)+4);
        end case;
        -- DEBUG-MUX
        case testMode_debug is
            when '1' => 
                instruction <= testInstruction_debug;
            when others => 
                instruction <= readInstruction;
        end case;
        case ramInsertMode_debug is
            when '1' =>
                RAM_writeEn <= ramWriteEn_debug;
                RAM_address <= ramWriteAddr_debug;
                RAM_data <= ramWriteData_debug;
            when others =>
                RAM_writeEn <= memWrite;
                RAM_address <= aluResult(11 downto 2);
                RAM_data <= reg2;
        end case;
    end process;
	-- Instruction Memory
	INSTR_ROM: entity ROrgPrSimLib.flashROM(behavioral)
		generic map(NUM_ELEMENTS => NUM_ROM_ELEMENTS,
		            LOG2_NUM_ELEMENTS => LOG2_NUM_ROM_ELEMENTS,
					ELEMENT_WIDTH => 32,
					INIT_FILE_NAME => PROG_FILE_NAME)
		port map(address => PC_now(11 downto 2),
				 readData => readInstruction);
	
	-- Data Memory
	invClk <= not clk;
	DATA_RAM: entity ROrgPrSimLib.flashRAM(behavioral)
		generic map(NUM_ELEMENTS => NUM_RAM_ELEMENTS,
					LOG2_NUM_ELEMENTS => LOG2_NUM_RAM_ELEMENTS,
					ELEMENT_WIDTH => RAM_ELEMENT_WIDTH,
					INIT_FILE_NAME => DATA_FILE_NAME)
		port map(clk => invClk,
				 address => RAM_address,
				 writeEn => RAM_writeEn,
				 writeData => RAM_data,
				 readEn => memRead,
				 readData => ramRead,
				 ramElements_debug => ramElements_debug);
				 
    -- DEBUG     
    pc_next_debug <= PC_next;
    CHAR_0: entity work.bin2Char(behavioral)
        port map(bin => PC_now(3 downto 0),
                bitmask => pc7SegDigits_debug(0));
    CHAR_1: entity work.bin2Char(behavioral)
        port map(bin => PC_now(7 downto 4),
                bitmask => pc7SegDigits_debug(1));
    CHAR_2: entity work.bin2Char(behavioral)
        port map(bin => PC_now(11 downto 8),
                bitmask => pc7SegDigits_debug(2));
    CHAR_3: entity work.bin2Char(behavioral)
        port map(bin => PC_now(15 downto 12),
                bitmask => pc7SegDigits_debug(3));

end structural;
