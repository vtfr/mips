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

	OPCode : in  STD_LOGIC_VECTOR(6 downto 0);
	Funct  : in  STD_LOGIC_VECTOR(6 downto 0);
	State  : in  STD_LOGIC_VECTOR(3 downto 0);

	vdd, vss: in STD_LOGIC
);
end control;

architecture behavioral of control is
	constant FETCH         : STD_LOGIC_VECTOR(3 downto 0) = "0000";
	constant DECODE        : STD_LOGIC_VECTOR(3 downto 0) = "0001";
	constant MEMADDR       : STD_LOGIC_VECTOR(3 downto 0) = "0010";
	constant MEMREAD       : STD_LOGIC_VECTOR(3 downto 0) = "0011";
	constant MEMWRITEBACK  : STD_LOGIC_VECTOR(3 downto 0) = "0100";
	constant MEMWRITE      : STD_LOGIC_VECTOR(3 downto 0) = "0101";
	constant EXECUTE       : STD_LOGIC_VECTOR(3 downto 0) = "0110";
	constant ALUWRITEBACK  : STD_LOGIC_VECTOR(3 downto 0) = "0111";
	constant BRANCH	       : STD_LOGIC_VECTOR(3 downto 0) = "1000";
	constant ADDIEXECUTE   : STD_LOGIC_VECTOR(3 downto 0) = "1001";
	constant ADDIWRITEBACK : STD_LOGIC_VECTOR(3 downto 0) = "1010";
	constant JUMP          : STD_LOGIC_VECTOR(3 downto 0) = "1011";

	signal State STD_LOGIC_VECTOR(3 downto 0) = FETCH;
begin
	process (Clk) begin
		if reset = "1" then
			State <= "0000";
			IorD <= '0';
			ALUSrcA <= '0';
			ALUSrcB <= "01";
			ALUOp <= "00";
			PCSrc <= "00";
			IRWrite	<= '1';
			PCWrite <= '1';

		elsif rising_edge(Clk) then
			case State is
				when FETCH =>
					IorD	<= '0';
					ALUSrcA <= '0';
					ALUSrcB <= "01";
					ALUOp   <= "00";
					PCSrc   <= "00";
					IRWrite	<= '1';
					PCWrite <= '1';
					State	 <= DECODE;

				when DECODE =>
					ALUSrcA <= '0';
					ALUSrcB <= "11";
					ALUOp   <= "00";

					with Opcode select State <=
						MEMADDR	    when 35,
						MEMADDR	    when 43,
						EXECUTE	    when 0,
						BRANCH	    when 4,
						ADDIEXECUTE when 9,
						JUMP        when 2;

				when MEMADDR =>
					ALUSrcA <= '1';
					ALUSrcB <= "10";
					ALUOp   <= "00";

					with Opcode select State <=
						MEMREAD  when 35,
						MEMWRITE when 43;

				when MEMREAD =>
					IorD  <= '1';
					State <= MEMWRITEBACK;

				when MEMWRITEBACK =>
					State <= FETCH;

					RegDst <= '0';
					MemtoReg <= '1';
					RegWrite <= '1';

				when MEMWRITE =>
					State <= FETCH;

					IorD <= '1';
					MemWrite <= '1';

				when EXECUTE =>
					State <= ALUWRITEBACK;

					ALUSrcA <= '1';
					ALUSrcB <= "00";
					ALUOp <= "10";

					with Funct select ALUControl <=
					 	"010" when 32,
						"110" when 34,
						"000" when 36,
						"001" when 37,
						"111" when 42;

				when ALUWRITEBACK =>
					State <= FETCH;

					RegDst <= '1';
					MemtoReg <= '0';
					RegWrite <= '1';

				when BRANCH =>
					State <= FETCH;

					ALUSrcA <= '1';
					ALUSrcB <= "00";
					ALUOp   <= "01";
					PCSrc   <= "01";
					Branch  <= '1';

				when ADDIEXECUTE =>
					State <= ADDIWRITEBACK;

					ALUSrcA <= '1';
					ALUSrcB <= "10";
					ALUOp   <= "00";

				-- ADDI Writeback
				when ADDIWRITEBACK =>
					State <= FETCH;

					RegDst   <= '0';
					MemtoReg <= '0';

				when JUMP =>
					State <= FETCH;

					PCSrc   <= "10";
					PCWrite <= '1';

				when others =>
					State <= FETCH;

			end case;
		end if;
	end process;
end;
