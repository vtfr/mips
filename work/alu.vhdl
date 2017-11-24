library IEEE;
use IEEE.STD_LOGIC_1164.all;
use IEEE.STD_LOGIC_SIGNED.all;

entity alu is
port (
	a, b  : in  STD_LOGIC_VECTOR(31 downto 0);
	s     : out STD_LOGIC_VECTOR(31 downto 0);
	ctrl  : in  STD_LOGIC_VECTOR(2 downto 0);
    zero  : out STD_LOGIC;

	vdd, vss: in STD_LOGIC
);
end alu;

architecture behavioral of alu is
	signal s_add, s_sub, s_and, s_or: STD_LOGIC_VECTOR(31 downto 0);
begin
	s_add <= a + b;
	s_sub <= a - b;
	s_and <= a and b;
	s_or  <= a or b;

	with ctrl select s <=
		s_add when "000",
		s_sub when "001",
		s_and when "010",
		s_or  when "110";

	zero <= '1' when s = x"00000000" else '0';
end;
