---
title: "The Second-System Effect: Explained"
date: "2018-05-09"
categories:
  - "software-engineering"
tags:
  - "architect"
  - "design"
  - "engineering"
  - "frederick-brooks"
  - "programming"
  - "software"
  - "tao-te-ching"
  - "the-mythical-man-month"
  - "the-second-system-effect"
---

The second-system effect is a well-known buzz word in the world of software engineering. As many years after it was first described by Frederick Brooks in his classic [The Mythical Man-Month](#the-reference), people are still talking about it. However, Dr. Brooks didn't actually give a rigorous definition about it, which certainly caused some confusions. What does it really mean? Is it really as simple as the terminology suggested?

Let's find out. Shall we?

This is my first post of the second-system effect series. I'd highly recommend you to check out the other two.

{% include second-system-effect-navigation.html %}


# The quote

For ease of discussing, let's first quote what we know about the second-system effect from [The Mythical Man-Month<sup>\[1\]</sup>](#the-reference):

> An architect’s first work is apt to be spare and clean. He knows he doesn’t know what he’s doing, so he does it carefully and with great restraint.
>
> As he designs the first work, frill after frill and embellishment after embellishment occur to him. These get stored away to be used "next time." Sooner or later the first system is finished, and the architect, with firm confidence and a demonstrated mastery of that class of systems, is ready to build a second system.
>
> This second is the most dangerous system a man ever designs. When he does his third and later ones, his prior experiences will confirm each other as to the general characteristics of such systems, and their differences will identify those parts of his experience that are particular and not generalizable.
>
> The general tendency is to over-design the second system, using all the ideas and frills that were cautiously sidetracked on the first one.
>
> ...
>
> The second-system effect has another manifestation somewhat different from pure functional embellishment. That is a tendency to refine techniques whose very existence has been made obsolete by changes in basic system assumptions.


# The explanation

The term, second-system effect, itself suggests that it is an effect happens to the second system when certain conditions are satisfied. So our focus, in this post, is to find out what the effect, the second system and the conditions are.

## The conditions

### The spare and clean first work

Just as the ancient Chinese proverb told us[<sup>\[2\]</sup>](#the-reference):

> A journey of a thousand miles begins with a single step.

Every great software architect must start his/her design journey at some point. That's is the unavoidable architect's first work.

When we talking about the second-system effect, we are not concerning about every first work, instead, we only cares about the first work that is "apt to be spare and clean". This may looks trivial, but it is really an important concept.

For one aspect, we are not concerning about whether the first work is succeeded or failed, as "spare and clean" doesn't necessarily means success. When "sooner or later the first system is finished", the design could be spare and clean, but the project itself may be considered failed as it missed the original due date.

For another, the consequence of such first work is the "firm confidence", the "demonstrated mastery of that class of systems", and the saved goodies "to be used 'next time'". These merits, come with the "spare and clean" first work, are the factors which make the second-system effect happen. In other word, the second-system effect won't occur, without a "spare and clean" first work.

### The class of systems

Apparently, a designer could have many works of different classes. However, for the second-system effect, an important point is that the first system and later ones must be of the same class. Otherwise, the heritage of the first system could hardly have any effect on the architect's later designs of a totally different domain. For example, a search engine is hardly considered as the second-system of an operating system, even if they are built by the same designer.

## The second system

Similarly, not every second system is our concern, the ones we are particularly interested in are those that follows almost immediately after the architects' first "spare and clean" work of the same class.

### The designer's experience

First, the word, second-system, we used in this context emphasizes in the experience of the designer. This is certainly one of the areas that caused [a lot of confusion]({{page.next.url}}). In other word, it doesn't matter which generation of the new system is, it only matters about how much experience the designer has when he/she works on the current project.

### The second

Another place where people usually misunderstand is that they think the second doesn't really mean the one after the first. However, In the essay, Dr. Brooks explicitly said the second-system is actually the designer's second work, not "the third or later ones". This means one designer can only experience the second-system effect at most once with one class of systems he/she works on. This also means one designer could experience the second-system effect more than once, during his/her career, if the class  of system he/she works on get changed.

### The influence of time

A lot of effects, in the world, are subject to the influence of time, and the second-system effect is certainly one of them. This is not clear stated in the article but I think it is implied. For example, one engineer designed a search engine, then left the domain to work on operating system designs, but 20 years later, he got a chance to work on search engine again. Would the second search engine design considered to be the second system of the first one? I doubt.

While, if another architect worked on a social media website, then temporally worked on a database, then back to work on a second social media website again. That would probably be considered as the case of the second system.

The point is that the influence of the first work will fade after time. So, at the time the designer is working on the second system, the influence of the first work should still be strong enough for the second-system effect to take place.

## The effect

The effect of the second-system is the tendency to over-design and to refine obsolete techniques. Similar to "spare and clean" doesn't mean the success of the first system, "over-design and refinement of obsolete techniques" doesn't necessarily mean the failure of the second system.

Over-design, or over-engineering, happens a lot. I think most C++ programmers has asked themselves questions like: Should we make the destructor virtual in order to make the future maintainers' job easier?  Should we use 64 bits integer for this counter to be extra safe or a 32 bits integer is good enough? The absolute right answer certainly depends on the situation. But to my experience, more often, they just doesn't really matter. No one cares or even notices you choose 64 bits over 32 bits, unless it shows up as a red flag on the performance measurement. Again, my point is that over-engineering happens a lot, and sometimes it just doesn't matter.

The same argument also applies to the refinement of obsolete techniques. Sure, that may take some time to do it. But if you do have plenty time and have nothing better to do, refine some obsolete techniques could still be fun and harmless.

## The architects

Another important aspect, which is not clear from the buzz word, the second-system effect, itself is that it could happen to an individual architect as while as a team, if that is the second system of the most designers. Note, the "most designers" here shouldn't be taken as the amount of the population, but rather it should be considered as the weight of the contribution to the project. This is mentioned by the Stretch example in the book. We will talk more about this [later]({{page.next.next.url}}).


# The epilogue

In this post, I have tried to explain what the second-system effect is by pointing out the key concepts about it. I hope you now have a better understand about the well-known buzz word. Let's end this post by giving a slightly better definition of the term:

**The second-system effect is the general tendency to over-design or to refine obsolete techniques when that system is the second system for most of the architects, whose first works of the same class are spare and clean.**

That's it. Simple, right? Unfortunately, this is not really the end of the story. The second-system effect could be more confusing than you thought. Wanna to know how? Stay tuned, for [my next post about the second-system effect]({{page.next.url}}).


# The reference

1. [Mythical Man-Month: Essays on Software Engineering, Anniversary Edition (2nd Edition) (#ad)](https://www.amazon.com) by Frederick Brooks.
1. [Tao Te Ching (#ad)](https://www.amazon.com) by Laozi.
