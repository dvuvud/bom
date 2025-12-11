# Projektets Git Guidelines

## Kodstil

* Projektet följer [Apache C coding conventions](https://httpd.apache.org/dev/styleguide.html).

## Branch-struktur

* Alla nya features, bugfixar, förbättringar etc. ska utvecklas i egna branches.
* Namngivning sker enligt formatet:

  * `feature/namn-pa-feature`
  * `bugfix/namn-pa-bugfix`
  * `hotfix/namn-pa-hotfix`

## Dokumentationsstil (.h filer)

Alla funktioner och datastrukturer som deklareras i `.h`-filer ska dokumenteras med Doxygen-stil.

Exempelmall att följa:

```
/**
 * @brief Kort sammanfattning av vad funktionen/datastrukturen gör
 * 
 * @param exempelParameter Beskrivning av parametern
 * @return Vad funktionen returnerar (om tillämpligt)
 * 
 * @note Viktiga notes att ta hänsyn till
 * @warning Om det finns några varningar eller potentiella sidoeffekter
 * @example
 * // Exempel användning
 * auto result = functionName(param);
 */
```

Vi behöver inte ha med allt detta när vi skriver dokumentation,
men det underlättar för oss alla när allt ska integraras senare.

Man får avgöra lite själv vad man anser är nödvändigt i dokumentationen

---

## Git Kommandoguide (snabbstart)

| Kommando                                  | Beskrivning                           | Exempel                                               |
| ----------------------------------------- | ------------------------------------- | ----------------------------------------------------- |
| `git clone <repo-url>`                    | Klonar projektet till din dator       | `git clone https://example.com/repo.git`              |
| `git checkout -b <branch-namn>`           | Skapar och byter till ny branch       | `git checkout -b feature/login-system`                |
| `git checkout <branch>`                   | Byter branch                          | `git checkout main`                                   |
| `git pull`                                | Hämtar senaste ändringar från remote  | `git pull`                                            |
| `git add .`                               | Lägger till alla ändringar för commit | `git add .`                                           |
| `git add <fil>`                           | Lägger till specifik fil för commit   | `git add main.cpp`                                    |
| `git commit -m "meddelande"`              | Skapar en commit                      | `git commit -m "Fixade bug i UI"`                     |
| `git push`                                | Laddar upp dina commits till remote   | `git push`                                            |
| `git push --set-upstream origin <branch>` | Pushar första gången på ny branch     | `git push --set-upstream origin feature/login-system` |
| `git status`                              | Visar vilka filer som ändrats         | `git status`                                          |
| `git log --oneline`                       | Visar commit-historiken               | `git log --oneline`                                   |
| `git merge <branch>`                      | Mergear in en branch i nuvarande      | `git merge feature/new-ui`                            |

### Workflow exempel

1. `git pull` (hämta senaste)
2. `git checkout -b feature/namn` (skapa branch)
3. Arbeta och committa:

   ```bash
   git add .
   git commit -m "Implementerade X"
   ```
4. `git push origin namn-på-branch`
5. Skapa en Pull Request & be om review

---
