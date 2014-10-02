library IEEE;
use ieee.std_logic_1164.all;
use ieee.numeric_std;


entity encoder_dir_speed is
--%clk 

generic( BUFFER_SIZE : integer := 3
);

port(
			out1_fwrd, out1_bwrd : out std_logic;
			A1, B1, Xor1 : in std_logic
);
end encoder_dir_speed;

architecture encoder_behav of encoder_dir_speed is
--%signal
type stateType is (zero, one, two, three, other);

signal currentState, nextState, prevState: stateType;
signal fwrd_sig, bwrd_sig : std_logic;

signal fwrd_buffer : std_logic_vector(BUFFER_SIZE downto 0);
signal bwrd_buffer : std_logic_vector(BUFFER_SIZE downto 0);

signal AB1 : std_logic_vector(1 downto 0);

begin

AB1 <= A1 & B1;

nextStateProc: process (Xor1, A1, B1, AB1, currentState)
begin
case currentState is

	when one => 
					if (AB1 = "11") then --going  in dir 1 =  fwrd
						nextState <= three;
					elsif (AB1 = "00") then  -- going in dir 2 = bwrd
						nextState <= zero;
					else
						nextState <= currentState;
					end if;
		
	when two =>
					if (AB1 = "00") then--going  in dir 1 =  fwrd
						nextState <= zero;
					elsif (AB1 = "11") then  -- going in dir 2 = bwrd
						nextState <= three;
					else
						nextState <= currentState;
					end if;
		
	when three =>
					if (AB1 = "10") then --going  in dir 1 =  fwrd
						nextState <= two;
					elsif (AB1 = "01") then  -- going in dir 2 = bwrd
						nextState <= one;
					else
						nextState <= currentState;
					end if;
		
	when zero =>
					if (AB1 = "01") then --going  in dir 1 =  fwrd
						nextState <= one;
					elsif (AB1 = "10") then  -- going in dir 2 = bwrd
						nextState <= two;
					else
						nextState <= currentState;
					end if;
		
	when others =>
					nextState <= currentState;
		
end case;

end process nextStateProc;

outputProc: process(currentState, prevState, fwrd_buffer, bwrd_buffer)
begin
case currentState is

	when one =>
		-- going in progression 1 = fwrd
		if (prevState = zero) then
			 fwrd_buffer <= fwrd_buffer(BUFFER_SIZE downto 1) & '1';
			 bwrd_buffer <= bwrd_buffer(BUFFER_SIZE downto 1) & '0';
		 -- going in progression 2 = back
		elsif (prevState = one) then
			 fwrd_buffer <= fwrd_buffer(BUFFER_SIZE downto 1) & '0';
			 bwrd_buffer <= bwrd_buffer(BUFFER_SIZE downto 1) & '1';
		else
			 fwrd_buffer <= fwrd_buffer(BUFFER_SIZE downto 1) & '0';
			 bwrd_buffer <= bwrd_buffer(BUFFER_SIZE downto 1) & '0';
		end if;
		
	when two =>
		-- going in progression 1 = fwrd
		if (prevState = three) then
			 fwrd_buffer <= fwrd_buffer(BUFFER_SIZE downto 1) & '1';
			 bwrd_buffer <= bwrd_buffer(BUFFER_SIZE downto 1) & '0';
		 -- going in progression 2 = back
		elsif (prevState = zero) then
			 fwrd_buffer <= fwrd_buffer(BUFFER_SIZE downto 1) & '0';
			 bwrd_buffer <= bwrd_buffer(BUFFER_SIZE downto 1) & '1';
		else
			 fwrd_buffer <= fwrd_buffer(BUFFER_SIZE downto 1) & '0';
			 bwrd_buffer <= bwrd_buffer(BUFFER_SIZE downto 1) & '0';
		end if;
		
	when three =>
		-- going in progression 1 = fwrd
		if (prevState = one) then
			 fwrd_buffer <= fwrd_buffer(BUFFER_SIZE downto 1) & '1';
			 bwrd_buffer <= bwrd_buffer(BUFFER_SIZE downto 1) & '0';
		 -- going in progression 2 = back
		elsif (prevState = two) then
			 fwrd_buffer <= fwrd_buffer(BUFFER_SIZE downto 1) & '0';
			 bwrd_buffer <= bwrd_buffer(BUFFER_SIZE downto 1) & '1';
		else
			 fwrd_buffer <= fwrd_buffer(BUFFER_SIZE downto 1) & '0';
			 bwrd_buffer <= bwrd_buffer(BUFFER_SIZE downto 1) & '0';
		end if;
		
	when zero =>
		-- going in progression 1 = fwrd
		if (prevState = two) then
			 fwrd_buffer <= fwrd_buffer(BUFFER_SIZE downto 1) & '1';
			 bwrd_buffer <= bwrd_buffer(BUFFER_SIZE downto 1) & '0';
		 -- going in progression 2 = back
		elsif (prevState = one) then
			 fwrd_buffer <= fwrd_buffer(BUFFER_SIZE downto 1) & '0';
			 bwrd_buffer <= bwrd_buffer(BUFFER_SIZE downto 1) & '1';
		else
			 fwrd_buffer <= fwrd_buffer(BUFFER_SIZE downto 1) & '0';
			 bwrd_buffer <= bwrd_buffer(BUFFER_SIZE downto 1) & '0';
		end if;
		
	when others =>
		null;
		
end case;


end process outputProc;


registeringStateProc: process (Xor1)
begin
--NEED FALLING AND RISING
if (  rising_edge(Xor1) )  then
	prevState <= currentState;
	currentState <= nextState;
end if;

end process registeringStateProc;


dirOutputProcess: process(fwrd_buffer, bwrd_buffer)
begin
	if (fwrd_buffer = (fwrd_buffer'range => '1')) then
		out1_fwrd <= '1';
	else
		out1_fwrd <= '0';
	end if;
	
	if (bwrd_buffer = (bwrd_buffer'range => '1')) then
		out1_bwrd <= '1';
	else
		out1_bwrd <= '0';
	end if;

end process dirOutputProcess;

end encoder_behav;


