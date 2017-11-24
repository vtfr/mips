library IEEE;
use IEEE.STD_LOGIC_1164.all;

entity control is
port (
	Clk   : in STD_LOGIC;
	Reset : in STD_LOGIC;

	MemToReg : out STD_LOGIC;
	RegDst   : out STD_LOGIC;
	IorD	 : out STD_LOGIC;
	PCSrc    : out STD_LOGIC_VECTOR(1 downto 0);

	ALUOP    : out STD_LOGIC_VECTOR(1 downto 0);
	ALUSrcA  : out STD_LOGIC;
	ALUSrcB  : out STD_LOGIC_VECTOR(1 downto 0);
	ALUCtrl  : out STD_LOGIC_VECTOR(2 downto 0);

	IRWrite  : out STD_LOGIC;
	MemWrite : out STD_LOGIC;
	PCWrite  : out STD_LOGIC;
	RegWrite : out STD_LOGIC;
	Branch   : out STD_LOGIC;

	OPCode : in  unsigned(6 downto 0);
	Funct  : in  unsigned(5 downto 0);

	vdd, vss: in STD_LOGIC
);
end control;

architecture behavioral of control is
	constant CONTROL_FETCH         : STD_LOGIC_VECTOR(3 downto 0) := "0000";
	constant CONTROL_DECODE        : STD_LOGIC_VECTOR(3 downto 0) := "0001";
	constant CONTROL_MEMADDR       : STD_LOGIC_VECTOR(3 downto 0) := "0010";
	constant CONTROL_MEMREAD       : STD_LOGIC_VECTOR(3 downto 0) := "0011";
	constant CONTROL_MEMWRITEBACK  : STD_LOGIC_VECTOR(3 downto 0) := "0100";
	constant CONTROL_MEMWRITE      : STD_LOGIC_VECTOR(3 downto 0) := "0101";
	constant CONTROL_EXECUTE       : STD_LOGIC_VECTOR(3 downto 0) := "0110";
	constant CONTROL_ALUWRITEBACK  : STD_LOGIC_VECTOR(3 downto 0) := "0111";
	constant CONTROL_BRANCH	       : STD_LOGIC_VECTOR(3 downto 0) := "1000";
	constant CONTROL_ADDIEXECUTE   : STD_LOGIC_VECTOR(3 downto 0) := "1001";
	constant CONTROL_ADDIWRITEBACK : STD_LOGIC_VECTOR(3 downto 0) := "1010";
	constant CONTROL_JUMP          : STD_LOGIC_VECTOR(3 downto 0) := "1011";
	
	signal State : STD_LOGIC_VECTOR(3 downto 0) := CONTROL_FETCH;
begin
	process (Clk, Reset) begin
		if Reset = "1" then
			State    <= CONTROL_FETCH;
			IorD     <= "0";
			ALUSrcA  <= "0";
			ALUSrcB  <= "01";
			ALUOp    <= "00";
			PCSrc    <= "00";
			IRWrite	 <= "1";
			PCWrite  <= "1";
			MemToReg <= "0";
			RegDst   <= "0";
			ALUCtrl  <= "000";
			MemWrite <= "0";
			RegWrite <= "0";
			Branch   <= "0";
			
		elsif rising_edge(Clk) then
			case State is
				when CONTROL_FETCH =>
					IorD	<= "0";
					ALUSrcA <= "0";
					ALUSrcB <= "01";
					ALUOp   <= "00";
					PCSrc   <= "00";
					IRWrite	<= "1";
					PCWrite <= "1";
					State	 <= CONTROL_DECODE;

				when CONTROL_DECODE =>
					ALUSrcA <= "0";
					ALUSrcB <= "11";
					ALUOp   <= "00";

					   if Opcode = 35 then State <= CONTROL_MEMADDR;
					elsif Opcode = 43 then State <= CONTROL_MEMADDR;
					elsif Opcode = 0  then State <= CONTROL_EXECUTE;
					elsif Opcode = 4  then State <= CONTROL_BRANCH;
					elsif Opcode = 9  then State <= CONTROL_ADDIEXECUTE;
					elsif Opcode = 2  then State <= CONTROL_JUMP;
					end if;

				when CONTROL_MEMADDR =>
					ALUSrcA <= "1";
					ALUSrcB <= "10";
					ALUOp   <= "00";

					   if Opcode = 35 then State <= CONTROL_MEMREAD;
					elsif Opcode = 43 then State <= CONTROL_MEMWRITE;
					end if;

				when CONTROL_MEMREAD =>
					IorD  <= "1";
					State <= CONTROL_MEMWRITEBACK;

				when CONTROL_MEMWRITEBACK =>
					State <= CONTROL_FETCH;

					RegDst <= "0";
					MemtoReg <= "1";
					RegWrite <= "1";

				when CONTROL_MEMWRITE =>
					State <= CONTROL_FETCH;

					IorD <= "1";
					MemWrite <= "1";

				when CONTROL_EXECUTE =>
					State <= CONTROL_ALUWRITEBACK;

					ALUSrcA <= "1";
					ALUSrcB <= "00";
					ALUOp <= "10";

					   if Funct = 32 then ALUCtrl <= "010";
					elsif Funct = 34 then ALUCtrl <= "110";
					elsif Funct = 36 then ALUCtrl <= "000";
					elsif Funct = 37 then ALUCtrl <= "001";
					elsif Funct = 42 then ALUCtrl <= "111";
					else ALUCtrl <= "000";
					end if;
				
				when CONTROL_ALUWRITEBACK =>
					State <= CONTROL_FETCH;

					RegDst <= "1";
					MemtoReg <= "0";
					RegWrite <= "1";

				when CONTROL_BRANCH =>
					State <= CONTROL_FETCH;

					ALUSrcA <= "1";
					ALUSrcB <= "00";
					ALUOp   <= "01";
					PCSrc   <= "01";
					Branch  <= "1";

				when CONTROL_ADDIEXECUTE =>
					State <= CONTROL_ADDIWRITEBACK;

					ALUSrcA <= "1";
					ALUSrcB <= "10";
					ALUOp   <= "00";

				-- ADDI Writeback
				when CONTROL_ADDIWRITEBACK =>
					State <= CONTROL_FETCH;

					RegDst   <= "0";
					MemtoReg <= "0";

				when CONTROL_JUMP =>
					State <= CONTROL_FETCH;

					PCSrc   <= "10";
					PCWrite <= "1";

				when others =>
					State <= CONTROL_FETCH;

			end case;
		end if;
	end process;
end;
