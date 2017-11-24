library IEEE;
use IEEE.STD_LOGIC_1164.all;

entity shift2 is
port (
	A : in STD_LOGIC_VECTOR(31 downto 0);
	S : out STD_LOGIC_VECTOR(31 downto 0);

	vdd, vss : in STD_LOGIC
);
end shift2;

architecture behavioral of shift2 is
begin
	S <= A(29 downto 0) & "00";
end;
