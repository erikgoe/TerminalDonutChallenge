# TerminalDonut challenge
Inspired by https://www.youtube.com/watch?v=sW9npZVpiMI

Original work: https://www.a1k0n.net/2011/07/20/donut-math.html

## The challenge
How long does it take for you to write a terminal program from scratch, which renders the same (or a similar) donut as above? No stackoverflow, no libraries (except terminal IO and trigonometric functions), no other sources. No preparation time. I did not read Andy's blog post before i started, just watched Joma's video. It's only allowed to look up elementary things (like how to clear the terminal window). Use any programming language you want. That said, let's start!

## The result
![The result](https://github.com/erikgoe/TerminalDonutChallenge/result.png "The result")

It took me solid 3 hours and 19 minutes to complete the challenge (until the output looked similar to Andy's implementation). Actually I thought it would only take one hour max, but as you probably know time estimation is a major weakness of most (if not all) programmers. Fixing the math and my own mistakes took by far the most time, but tweaking the numbers to match the result was also time-consuming. I am happy with the result now, even if the code is not optimal at all and rather chaotic. 

After all this little experiment was interesting and I really learned some stuff, so I'm glad I stumbled over the idea. Try it out yourself if you're curious, but stick to the rules!

### How to run the code
    g++ terminalDonut.cpp -o terminalDonut
    ./terminalDonut
