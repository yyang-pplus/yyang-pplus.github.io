---
title: "The Second-System Effect: Misunderstood"
date: "2018-05-10"
categories:
  - "software-engineering"
tags:
  - "architect"
  - "design"
  - "engineering"
  - "frederick-brooks"
  - "programming"
  - "software"
  - "the-mythical-man-month"
  - "the-second-system-effect"
---

The second-system effect is a well-known buzz word in the world of software engineering. As many years after it was first described by Frederick Brooks in his classic [The Mythical Man-Month](#the-reference), people are still talking about it. Unfortunately, the terminology, chose by Dr. Brooks, is kind of misleading. When people do talk about it, do they really refer to the same thing?

Let's find out. Shall we?

This is my second post of the second-system effect series. I'd highly recommend you to check out the other two.

{% include second-system-effect-navigation.html %}


# The second system vs the second generation

A common misunderstanding about the second-system effect, is failed to distinguish the second system with the second generation of the same system.

For example, the following statement from [Avoiding the Second System Effect in Software Development](http://www.25hoursaday.com/weblog/2008/08/04/AvoidingTheSecondSystemEffectInSoftwareDevelopment.aspx):

> In my experience, the second system effect doesn't just come into play when the creator(s) of the second system also worked on the first system.

This statement could only be true when the second system also means the second generation. Otherwise, it is totally a fallacy. How could an architect create the second system without designing his first one. However, even in that case, this statement is still missing the point.

The term second system we used in the context of the second-system effect emphasizes on the experience of "the creator(s)". It is true that "the creator(s)" of the second generation system doesn't have to also worked on the first generation, but they have to have experience with one system of the same class. Otherwise, the precondition, "an architect’s first work is apt to be spare and clean", that leads to the result of the second-system effect, won't stand.


# The second ≠ 2.0

This is a variation of essentially the same issue we talked about in the [previous section](#the-second-system-vs-the-second-generation).

The version 2.0 of a system is not necessarily means the designer's second system, not even necessarily means the second generation of the same system. For example, the "more modern description" of the second-system effect in [Hack, HHVM and avoiding the Second-system effect](http://bytepawn.com/hack-hhvm-second-system-effect.html).

For example, as of Feb 21, 2018, the version number of the Facebook App for iOS reached 160.0. Do you really think that is the 160th generation of the system? I doubt. My point is that the version number is kind of arbitrary, it is almost impossible to tell about the design of a system only from its version number for anyone without inside knowledge.

Even when version 2.0 does means the same thing as the second generation, it is still hard to tell, as we have discussed in the [previous section](#the-second-system-vs-the-second-generation), if it is the designer's second system.


# The third-system syndrome

In the essay, The Second-System Effect, Dr. Brooks clearly said the second system is actually the designer's second work, not "the third or later ones", of the same class. But why people still think the effect could happen on the later ones. Such as mentioned in [Beware The Second System Effect](http://robertgreiner.com/2012/07/the-second-system-effect/):

> Personally, I think the perils of The Second System Effect can exist well beyond your second project and this is something we always have to be on guard against.

There are a few possibilities that I can think of:

One of them is, "your second project" may not be the same class of the first one. As I have already shown in my [previous post]({{page.previous.url}}), one of the precondition that leads to the second-system effect is that, the designer's second system have to be the same class of the first one. Otherwise, the confidence, familiarity, and saved frills, from the first work, could hardly have any effect on the architect's later designs of a totally different domain. In this sense, it may be true that "the perils of The Second System Effect can exist well beyond your second project", if you keep changing the classes of your systems.

Another possibility is, "your second project" of the same class, may not be following a spare and clean first work. What would happen to the second system that follows a first system that is not spare or clean? What if you still do not have much confidence when works on your second project? Is it possible that the danger of the second-system will be delayed to occur on one of the designer's later projects? Questions like this are left unanswered in Dr. Brooks' essay.

One final possibility is, you are not the only architect that works on "your second project". One important aspect of the second-system effect is that it could happen to an individual architect as while as a team, if that is the second system of the most designers. In that case, "the perils of The Second System Effect can exist well beyond your second project", if that project happens to be the second system of the majority designers.

So, in my opinion, thinking the second-system effect could happen on one's third or later projects, is at least misleading if not misunderstanding.


# The summary

In this post, I have listed a few common misused, but important, aspects of the second-system effect.

In short, **the second-system effect is emphasized on the experience of the majority of architects involved in the design of the second system, which follows a spare and clean first one of the same class.**

Probably, you have gained a better understanding of the buzz word by now. However, don't be satisfied yet, I will challenge the concept of the second-system effect even more in [my next post]({{page.next.url}}). So, stay tuned.


# The reference

1. [Mythical Man-Month: Essays on Software Engineering, Anniversary Edition (2nd Edition) (#ad)](https://www.amazon.com) by Frederick Brooks.
