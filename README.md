# Vigenere
Decryption algorithm I created and used to decrypt some ciphers given in the class

## Explanation:
Here are the 3 steps of my solution:

1.  **Guessing the key-length:** Here I used shifting method. As a
    shifting, I used right-shifting. After each shifting process, I am
    counting the letters that are in the same position as they were
    before shifting. After several shifting, I am finding the shifting
    numbers in which the repetition was the highest. After found, we can
    easily see the pattern, and guess the keylength. I tried to find the
    exact *key-length*, and in the cases that I tested, it was
    successful: I found the highest repetition number, say `n`, then I
    listed the shifting numbers in which the repetition number was more
    than or equal to `9 * n / 10`, then I found the gcd of those
    listed numbers, and it was key-length. I coded it to automatically
    find the *key-length* as well but also gave the chance to reenter
    the shift number if user wants to make the another guess;

2.  **Finding the key:** It is the most interesting part of the story. I
    used here numerical method. First, from the following link I got the
    frequency of English letters: [Letter
    frequency](http://en.wikipedia.org/wiki/Letter_frequency).\
    Then I created the global array consisting of frequency of letters
    according to that given frequency. I multiplied the appropriate
    frequency percentage by `1000`, and rounded for being integer; e.g.
    `b` has frequency `1.492%`, in my array the appropriate
    value is `1492`. And the total sum of the elements of the global
    array is `99,999`, as you see in my source codes.\
    After finding the *key-length*, I divided the ciphertext into the
    block having the size of *key-length*. Each block has the columns
    numbered from `0, 1, ..., (key-length - 1)`; so, I grouped the letters in
    the `k`th column of each block, then in each group I found the
    letter frequency for all members of the group. Let’s name the groups
    as `W_0, W_1, ..., W_{key-length - 1}`. Now I will compare the
    frequency groups with the standart English letter frequency table.
    For this I used residual methods, but for this I need to adjust the
    group members’ frequencies array that I created; i.e. it should have
    the same total sum with others for simplicity. For this purpose, I
    did the following operation with rounding for integer:
    ```
    for i in 0, 1, ..., 25
    	for j in 0, 1, ..., (key-length - 1)
    		99999 * W_j(i) / |W_j|
    	end
    end
    ```
    Then, assuming `A` is the standard English letter frequency array, I found the residuals for each group:
    ```
    for i in 0, 1, ..., 25
    	residual_i = 0;
    	for j in 0, 1, ..., 25
    		residual_i += (A(j) - W_i(j))^2
    	end
    end
    ```
    Then I shifted the groups I am tested for refinding residual; so,
    for each group I put all `26` residual values in vector after
    shifting. Then I find the minimum shift number in which the residual
    is minimum; that number is the corresponding *key-letter*;

3.  **Decrypting:** Follow the previous parts, and find the key. Then
    divided the text into the blocks each of which has length of
    *key-length*. Then subtract the key from each block. After all
    combine all subtracted parts in order to get the decrypted message.

**Thank you for taking a look at my explanation!**