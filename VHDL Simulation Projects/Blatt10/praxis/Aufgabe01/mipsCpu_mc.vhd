library ieee;
use ieee.std_logic_1164.all;
use ieee.numeric_std.all;

library ROrgPrSimLib;
use ROrgPrSimLib.proc_config.all;
use ROrgPrSimLib.all;

library work;
use work.all;


entity mipsCpu_mc is
	generic(INIT_FILE_NAME : string);
	port(clk : in std_logic;
		 rst : in std_logic;
		 
		 -- debug ports
		 ramElements_debug : out ram_elements_type;
		 registers_debug : out reg_vector_type;
		 mipsCtrlState_debug : out mips_ctrl_state_type;
		 pc7SegDigits_debug : out pc_7seg_digits_type
		 );
end mipsCpu_mc;

architecture structural of mipsCpu_mc is	
signal invClk : std_logic;

--Temporäre Register OUT
signal instruction : std_logic_vector(31 downto 0) := (others => '0');
signal memDataReg : std_logic_vector(RAM_ELEMENT_WIDTH-1 downto 0) := (others => '0');
signal aReg, bReg : std_logic_vector(REG_WIDTH-1 downto 0) := (others => '0');
signal aluOut : std_logic_vector(ALU_WIDTH-1 downto 0) := (others => '0');

--Ram-Ports
signal ramAddress : std_logic_Vector(LOG2_NUM_RAM_ELEMENTS-1 downto 0) := (others => '0');
signal memData : std_logic_vector(RAM_ELEMENT_WIDTH-1 downto 0) := (others => '0');

-- Steuersignale cycle 1
signal regDst, memRead, memToReg, memWrite, regWrite, aluSrcA, irWrite, IorD, pcWrite, pcWriteCond : std_logic := '0';
signal aluOp, aluSrcB, pcSrc : std_logic_vector (1 downto 0) := (others => '0');

--Signale ALU
signal aluInA, aluInB, aluResult : std_logic_vector(ALU_WIDTH-1 downto 0) := (others => '0');
signal aluZero : std_logic := '0';
signal aluControl : std_logic_vector(3 downto 0) := (others => '0');

--Signale Register I/O
signal regWriteAddr : std_logic_vector(LOG2_NUM_REGS-1 downto 0) := (others => '0');
signal regReadData1, regReadData2, regWriteData : std_logic_vector(REG_WIDTH-1 downto 0) := (others => '0');

--Signale PC
signal pcSignal : std_logic := '0';
signal pcNow, pcNext : std_logic_vector(PC_WIDTH-1 downto 0) := (others => '0');

--Signale Signextend
signal siExt : signed(31 downto 0) := (others => '0');

--Signale Shift nach Signextend
signal siExtShift : std_logic_vector(31 downto 0) := (others => '0');

--Signale Jumpshifter
signal jumpShift, jumpShiftIn : std_logic_vector(27 downto 0) := (others => '0');
begin

	-- Beschreibung der Multicycle-MIPS-CPU hier ergänzen
	invClk <= not clk;
	pcSignal <= pcWrite or (pcWriteCond and aluZero);
	jumpShiftIn <= "00"&instruction(25 downto 0);
	MUX_AND_PC: process(pcSignal, pcNext, pcSrc, aluResult, aluOut, pcNow, jumpShift, aluSrcA, aReg, aluSrcB, bReg, siExt, siExtShift, regDst, instruction, memToReg, memDataReg, IorD, clk)
	begin
            -- PC
            if (rst = '1') then
                pcNow <= (others => '0');
            elsif (clk'EVENT and clk = '1' and pcSignal='1') then
                pcNow <= pcNext;
            end if;
            
            --MUX: pcNext
            case pcSrc is
                when "00" =>
                    pcNext <=aluResult;
                when "01" =>
                    pcNext <= aluOut;
                when others =>
                    pcNext <= pcNow(31 downto 28)&jumpShift;
            end case;
            --MUX: aluInA
            case aluSrcA is
                when '0' =>
                    aluInA <= pcNow;
                when others =>
                    aluInA <= aReg;
            end case;
            --MUX: aluInB
            case aluSrcB is
                when "00" =>
                    aluInB <= bReg;
                when "01" =>
                    aluInB <= std_logic_vector(to_Signed(4, ALU_WIDTH));
                when "10" =>
                    aluInB <= std_logic_vector(siExt);
                when others =>
                    aluInB <= siExtShift;
            end case;
            --MUX: regWriteAddr
            case regDst is
                when '0' =>
                    regWriteAddr <= instruction(20 downto 16);
                when others =>
                    regWriteAddr <= instruction(15 downto 11);
            end case;
            --MUX. regWriteData
            case memToReg is
                when '0' =>
                    regWriteData <= aluOut;
                when others =>
                    regWriteData <= memDataReg;
            end case;
            --MUX: ramAddress
            case IorD is
                when '0' =>
                    ramAddress <= pcNow(11 downto 2);
                when others =>
                    ramAddress <= aluOut(11 downto 2);
            end case;
	end process;
	
	CONTROL: entity work.mipsCtrlFsm(behavioral)
            port map(clk => clk,
                    rst => rst,
                    op => instruction(31 downto 26),
                    regDst => regDst,
                    memRead => memRead,
                    memToReg => memToReg,
                    aluOp => aluOp,
                    memWrite => memWrite,
                    regWrite => regWrite,
                    aluSrcA => aluSrcA,
                    aluSrcB => aluSrcB,
                    pcSrc => pcSrc,
                    irWrite => irWrite,
                    IorD => IorD,
                    pcWrite => pcWrite,
                    pcWriteCond => pcWriteCond,
                    mipsCtrlState_debug => mipsCtrlState_debug);
                    
        ALU: entity work.csAlu(behavioral)
            generic map(WIDTH => ALU_WIDTH)
            port map(ctrl => aluControl,
                    a => aluInA,
                    b => aluInB,
                    result => aluResult,
                    overflow => open,
                    zero => aluZero);
                    
        ALU_CTRL: entity work.aluCtrl(behavioral)
            port map(aluOp => aluOp,
                    f => instruction(5 downto 0),
                    operation => aluControl);
                    
        REGISTRY: entity work.regFile(structural)
            generic map(NUM_REGS => NUM_REGS,
                        LOG2_NUM_REGS => LOG2_NUM_REGS,
                        REG_WIDTH => REG_WIDTH)
            port map(clk => clk,
                    rst => rst,
                    readAddr1 => instruction(25 downto 21),
                    readData1 => regReadData1,
                    readAddr2 => instruction(20 downto 16),
                    readData2 => regReadData2,
                    writeEn => regWrite,
                    writeAddr => regWriteAddr,
                    writeData => regWriteData,
                    reg_vect_debug => registers_debug);
                    
        INSTR_REG: entity work.regFile(structural)
            generic map(NUM_REGS => 1,
                        LOG2_NUM_REGS => 1,
                        REG_WIDTH => 32)
            port map(clk => clk,
                    rst => rst,
                    readAddr1 => "0",
                    readData1 => instruction,
                    readAddr2 => "0",
                    readData2 => open,
                    writeEn => irWrite,
                    writeAddr => "0",
                    writeData => memData,
                    reg_vect_debug => open);
                    
        MEM_REG: entity work.regFile(structural)
            generic map(NUM_REGS => 1,
                        LOG2_NUM_REGS => 1,
                        REG_WIDTH => RAM_ELEMENT_WIDTH)
            port map(clk => clk,
                    rst => rst,
                    readAddr1 => "0",
                    readData1 => memDataReg,
                    readAddr2 => "0",
                    readData2 => open,
                    writeEn => '1',
                    writeAddr => "0",
                    writeData => memData,
                    reg_vect_debug => open);
                    
        A_REG: entity work.regFile(structural)
            generic map(NUM_REGS => 1,
                        LOG2_NUM_REGS => 1,
                        REG_WIDTH => REG_WIDTH)
            port map(clk => clk,
                    rst => rst,
                    readAddr1 => "0",
                    readData1 => aReg,
                    readAddr2 => "0",
                    readData2 => open,
                    writeEn => '1',
                    writeAddr => "0",
                    writeData => regReadData1,
                    reg_vect_debug => open);
                    
        B_REG: entity work.regFile(structural)
            generic map(NUM_REGS => 1,
                        LOG2_NUM_REGS => 1,
                        REG_WIDTH => REG_WIDTH)
            port map(clk => clk,
                    rst => rst,
                    readAddr1 => "0",
                    readData1 => bReg,
                    readAddr2 => "0",
                    readData2 => open,
                    writeEn => '1',
                    writeAddr => "0",
                    writeData => regReadData2,
                    reg_vect_debug => open);
                    
        ALU_REG: entity work.regFile(structural)
            generic map(NUM_REGS => 1,
                        LOG2_NUM_REGS => 1,
                        REG_WIDTH => ALU_WIDTH)
            port map(clk => clk,
                    rst => rst,
                    readAddr1 => "0",
                    readData1 => aluOut,
                    readAddr2 => "0",
                    readData2 => open,
                    writeEn => '1',
                    writeAddr => "0",
                    writeData => aluResult,
                    reg_vect_debug => open);
	
        SIEXTER: entity work.signExtend(behavioral)
            generic map(INPUT_WIDTH => 16,
                        OUTPUT_WIDTH => 32)
            port map(number => signed(instruction(15 downto 0)),
                    signExtNumber => siExt);
                    
        EXTSHIFTER: entity work.leftShifter(behavioral)
            generic map(WIDTH => 32,
                        SHIFT_AMOUNT => 2)
            port map(number => std_logic_vector(siExt),
                    shiftedNumber => siExtShift);
                    
        JUMPSHIFTER: entity work.leftShifter(behavioral)
            generic map(WIDTH => 28,
                        SHIFT_AMOUNT => 2)
            port map(number => jumpShiftIn,
                    shiftedNumber => jumpShift);
                    
        PC_7SEG_0: entity work.bin2Char(behavioral)
            port map(bin => pcNow(3 downto 0),
                    bitmask => pc7SegDigits_debug(0));
        PC_7SEG_1: entity work.bin2Char(behavioral)
            port map(bin => pcNow(7 downto 4),
                    bitmask => pc7SegDigits_debug(1));
        PC_7SEG_2: entity work.bin2Char(behavioral)
            port map(bin => pcNow(11 downto 8),
                    bitmask => pc7SegDigits_debug(2));
        PC_7SEG_3: entity work.bin2Char(behavioral)
            port map(bin => pcNow(15 downto 12),
                    bitmask => pc7SegDigits_debug(3));
	-- Daten und Instruktionsspeicher
	INSTR_AND_DATA_RAM: entity ROrgPrSimLib.flashRAM(behavioral)
		generic map(NUM_ELEMENTS => NUM_RAM_ELEMENTS,
					LOG2_NUM_ELEMENTS => LOG2_NUM_RAM_ELEMENTS,
					ELEMENT_WIDTH => RAM_ELEMENT_WIDTH,
					INIT_FILE_NAME => INIT_FILE_NAME)
		port map(clk => invClk,
				 address => ramAddress,
				 writeEn => memWrite,
				 writeData => bReg,
				 readEn => memRead,
				 readData => memData,
				 ramElements_debug => ramElements_debug);
	
end structural;
