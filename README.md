Submitted: 05/01/24; grade: 96

For this lab, we had to take an input file of 100 strings of 16 characters and place them into a hash table. The "Burris" function was the original hash function which we had to critique, and then create our own more efficient hash function. We had to use both a linear and random probe technique and had to print out the statistics for the efficiency of the hash function used. I opted to use a boolean to select between my hash function and Burris' hash function.

Below is my critique of the Burris hash and the logic for my own:

There are a few key weaknesses with the hash function provided. I’ll repeat it here so it is easier to reference:

HA = abs{ str(2:2) + [ ( str(4:5) + str(7:8) ) / 381 + str(1:1) ] /587 – str(11) }

The hash function is heavily reliant on only a few characters of the hash function. Many words across English have similar letters at key points of a work, therefor only keeping in mind a few characters across a larger word would increase the total amount of collisions. A good hash function should use the entire string to increase randomness.
The addition and subtraction functions are weighted too heavily. If the character is too close to the ASCII value of “a”, the function will be clustered towards the top of the table, whereas if the ASCII is closer to “z”, it will be clustered towards the bottom. If the keys provided tend to start with letters closer to “a”, they will not be uniformly distributed, but rather cluster towards the top of the table.
The division constraints (381 and 587) don’t seem to have any relation in making sure the hash table is uniformly distributed. It would be better if they were more related to the hash table size or were smaller so that it could more uniformly distribute the keys, as larger numbers tend to cluster up the outputs. 587 is a prime number, which is generally good, due to prime numbers' nature of avoiding patterns, however, it would be better if it was coprime to the table size, so that every input could be mapped to a different slot in the table.
Upon inspection, there are some things the original hash function doesn’t implement that a great hash function uses
Utilization of a bitwise shift.
Bitwise shifts are extremely hardware-efficient ways to multiply numbers by powers of 2. This would effectively spread out the high and low bits of the function
Afterward, you could multiply it by a prime number to randomly distribute the key across the table.
Using a preset value for the starting point of the HA.
Starting from 0 leads to a lack of complexity, as well as issues when the HA is multiplied and divided, as multiplying and dividing by 0 is an automatic crash of the function. Utilizing addition from the starting point of 0 makes your hash function just a sum of ASCII values.
Daniel J Bernstein chose the number 5381 as a random prime number that yielded good results for fewer collisions. 
Therefore a better hash function should have the following qualities:
It should use every character in the string to increase randomness and avoid clustering
It should use bitwise operations to spread out the high and low bits of the function
It should use the addition of a prime number to uniformly distribute the key
It should be modded by the table size to fit that large number into the table while maintaining the random nature of the number generated
The original HA number will not be set to 0, instead being set to 5381 to better help distribution.

Here is the function I came up with:

int improvedHashFunction(const string& str) {
    long hash = 5381;
    for (char c : str) {
        hash = ((hash << 5) + hash) + c; // Equivalent to hash * 33 + c
    }
    return hash % TABLE_SIZE;  // Modulo table size to ensure it fits within the table
}

The function utilizes a for loop to iterate over each character in the string, then uses the bitwise operation to spread out the high and low bits of the function, then pulls from the string itself to make each key unique, then mods the HA to put the random number back into the table.
