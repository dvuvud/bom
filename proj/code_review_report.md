# Code Review Report

## Participant List
| Name                   | Email                                    | Active Dates           |
|------------------------|------------------------------------------|------------------------|
| David Buhrman          | david.buhrman.0648@student.uu.se         | 3/12 2025 – 16/1 2026  |
| Erik Verastegui Ochoa  | erik.verastegui-ochoa.3297@student.uu.se | 3/12 2025 – 16/1 2026  |
| Hiba Ahmad             | hiba.ahmad.5302@student.uu.se            | 3/12 2025 – 16/1 2026  |
| Joel Terenius          | joel.terenius.0417@student.uu.se         | 3/12 2025 – 16/1 2026  |
| Nora Ayaz              | nora.ayaz.1089@student.uu.se             | 3/12 2025 – 16/1 2026  |
| Veronica Pettersson    | veronica.pettersson.7655@student.uu.se   | 3/12 2025 – 16/1 2026  |

---

## 1. Overview

During the project, we tried to keep doing code reviews regularly. We used GitHub pull requests 
to go through each other’s code and see if the changes made sense and followed the requirements. 
This helped us understand what others were working on. 

When something did not work as expected, or when we noticed bugs or any strange behavior, or 
something someone couldn't solve on their own we opened a GitHub issue. Sometimes this happened 
during reviews, sometimes after testing. Using issues helped us remember what needed to be fixed 
and made it easier to talk about problems in the group. In general, this way of working helped us 
keep the project more organized and improved teamwork.

---

## 2. Code Review Workflow

We did all implementation work using feature branches, and pull requests. Everyone in the team worked
in this way. Before any pull request 
was merged into the main branch, it was reviewed by at least one other group member.

Pull requests were used for many different types of implementation and changes, such as:

* adding new functionality (for example allocation, reference counting, and cascading frees)
* fixing bugs
* adding or updating tests
* updating documentation and project reports
* improving code style and consistency

Every pull request was reviewed, and we added comments if something needed to be changed or corrected. 
If everything looked fine, the pull request was approved and merged. When reviewing, the colored diff
in github was helpful for finding what parts of the code that a pull request changes.

---

## 3. Examples of Code Review in Practice

In many cases, code reviews led to changes before the code was merged. Some examples of what we 
reviewed are:

* Pull requests related to allocate() and deallocate() were checked carefully to make sure they 
behaved correctly and matched the existing logic.
* Changes involving retain(), release(), and rc() were reviewed with focus on reference counting 
rules and memory safety.
* Pull requests related to cascading frees and the pending free queue reviewed with extra attention, 
since these parts affect how the whole system behaves.
* Some pull requests were about code style, such as fixing indentation, removing tab characters, 
or making helper functions more consistent.
* We reviewed also test-related pull requests to check every single functionality were tested 
and memory leaks found during testing were fixed.

---

## Review Focus Areas

We mainly focused on:

* correctness and logic
* memory safety
* consistency with reference-counting rules
* how different modules (refmem, queue, and hashset) interact with each other
* readability and naming
* test coverage

Changes that affected the core memory management logic usually received more detailed reviews.

---

## 4. Issue Tracking

We used GitHub Issues as part of our code review and code quality process. Issues were usually 
created when problems were found during reviews, testing, or while adding new features.

Some common reasons for creating issues were:

* missing or incorrect behavior, for example when cascading frees did not work as expected
* memory leaks that were discovered during testing
* bugs related to the default destructor
* problems with code style or consistency
* documentation that was outdated after code changes

We discussed every single issue within the group and then fixed through follow-up pull requests. 
An issue was only closed after the problem was fixed and checked. 

Using issues together with pull requests helped us keep track of problems, remember review feedback, 
and make sure that issues were not forgotten. Overall, this made the code review process more 
structured and efficient.

---

## Pull Request Statistics

In total, there were 47 pull requests in our project. Most of them were closed after 1 - 2 days,
or even on the same day, but a few pull requests could take closer to a week to close, especially
the ones dealing with a difficult design issue. 11 of the 47 pull requests got push back, which was
addressed before it could be merged into main.

---

## 5. Impact on Collaboration within the team

Pull requests and code reviews helped us work together. We looked at each other’s code, not only our 
own part. Sometimes this took extra time, and sometimes it was confusing, but it helped us understand 
more of the system.

We did not want only one person to know how things worked. By reviewing code, every member of the team
saw the changes and could ask questions. This made it easier to help each other. It also reduced the 
chance that someone changed something without the rest of the group noticing. 

---

## 6. Conclusion

Code reviews were part of how we worked in the project. We used pull requests to look at each other’s 
code and tests as well. This helped us find problems and talk about changes within the group.

This process helped us avoid bigger mistakes, and finish the project in a better way than if we worked alone.

We are happy with how we used code reviews in the project, and would probably do it the same way if we had to do it again.

---