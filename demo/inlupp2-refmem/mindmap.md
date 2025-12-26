# Tankekarta: Inlämningsuppgift 2 – Webstore

## 1. Förberedelser
- Skapa mappstruktur:
  - Data-structures/
  - Utils/
  - Webstore-backend/
  - Webstore-frontend/
  - Tests/
- Återanvänd listor, hash-tabeller och iteratorer från inlupp1
- Återanvänd utils.c, utils.h och db.c från labbarna
- Skapa README och Makefile
- Planera huvudfunktioner för backend och frontend

---

## 2. Backend – Funktioner att implementera

### Skapa och hantera lager
- `create_warehouse_hash()` – skapar hashtabell för varor  
- `destroy_warehouse_hash()` – frigör allt minne i lagret  
- `add_merchandise()` – lägger till en ny vara i lagret  
- `remove_merchandise()` – tar bort en vara helt  
- `edit_merchandise()` – ändrar namn, beskrivning eller pris  
- `list_merchandise()` – returnerar en lista med alla varunamn  
- `find_merchandise()` – söker upp vara med visst namn

### Hantera hyllor och lagerplatser
- `create_shelf()` – skapar ny hylla  
- `insert_shelf()` – lägger till hylla i rätt ordning  
- `find_shelf()` – söker efter specifik hylla  
- `replenish_stock()` – fyller på eller uppdaterar antal på en hylla  
- `remove_stock()` – minskar antal eller tar bort hylla helt  
- `remove_locations()` – tar bort alla hyllor kopplade till en vara  

### Kundvagnar
- `create_carts()` – skapar en struktur för alla kundvagnar  
- `destroy_carts()` – frigör minne för alla kundvagnar  
- `create_cart()` – skapar en ny tom kundvagn  
- `remove_cart()` – tar bort kundvagn med visst ID  
- `add_to_cart()` – lägger till vara i en kundvagn  
- `remove_from_cart()` – tar bort viss mängd av en vara  
- `find_cart_item()` – söker upp en vara i kundvagnen  
- `calculate_cost()` – beräknar total kostnad för kundvagn  
- `checkout_cart()` – genomför köp, minskar lager och tar bort kundvagnen  

### Avslutning
- `quit()` – frigör allt minne och stänger programmet

---

## 3. Frontend – Funktioner och menyval
- `main_menu()` – visar huvudmenyn och tar emot val  
- `action_add_merch()` – hanterar inmatning för att lägga till vara  
- `action_edit_merch()` – ändrar information för vald vara  
- `action_remove_merch()` – tar bort en vara  
- `action_list_merch()` – visar alla varor  
- `action_replenish()` – fyller på lager  
- `action_create_cart()` – skapar ny kundvagn  
- `action_remove_cart()` – tar bort kundvagn  
- `action_add_to_cart()` – lägger till vara i kundvagn  
- `action_remove_from_cart()` – tar bort vara från kundvagn  
- `action_checkout()` – avslutar köp  
- `quit()` – avslutar programmet

---

## 5. Testning
- Skriv CUnit-tester för varje huvudfunktion i backend ex:
  - `test_create_merch()`
  - `test_add_merchandise()`
  - `test_edit_merchandise()`
  - `test_remove_merchandise()`
  - `test_replenish_stock()`
  - `test_create_cart()`
  - `test_add_to_cart()`
  - `test_remove_from_cart()`
  - `test_calculate_cost()`
  - `test_checkout_cart()`
- Kör tester med:
  - `make test`
  - `make valgrind`
  - `make coverage`

---

## 6. Avslutning
- Kontrollera kodtäckning 
- Säkerställ att inga minnesläckor finns
- Rensa med `make clean`
- Tagga sista commit som `assignment2_done`
- Lämna in färdigt projekt
