# Milo_Knight_Game
 It's Mi-lo(Minh & Long) programing project base on Soul Knight's game play built with SDL1.2 and C++.
 ## Window Installation
 - First, install Visual Studio 2012.
 - Clone this repo to your computer.
 - Delete everything in checkpoint.txt
 - Open SDL1_2Game.sln
 - Press `F5` and enjoy the game xD.
## Gameplay
- You have to control player to clear enermies in each room. Enemies can shoot projectiles, and have collider damage.
- You can control player by using `←↑→↓`.
- Your Player has 5 statics:
   + Health : it will be decrease when an Enemy hit you, if your Health `equal to 0`, you will die and `Game Over`.
   + Money : Increase each times you defeat an enemy. Using Money in `store` to buy new items.
   + Shield : If your shield index is `greater than 0`, each enemy shot will not reduce your health index but will reduce your shield index. Your shield index recover each times you go to new room.
   + AD power : A static that can increase by buying new items.
   + AP power : A static that can increase by buying new items.
- Your player has 3 abilities:
  + Using `Q` to fire a damaging bolt of energy scale by your AD power
  + Using `E` to fire an orb that sign the enemy hit in 3 seconds . If player hits an signed enemy by another abilities, unsign and deals damage scale by your AP power.
  + Using `SPACE` to fire a powerful barrage of energy that deals damage to each unit it passes through scale by your AP power.
## Store & Items(Not yet include, please wait for next update)
- Each times before you reach to an room, Store will be open, you can use your money to buy new items.
## About Game
- This Game is built with OOP system which I learned from [`here`](https://www.youtube.com/@PhatTrienPhanMem123AZ).
## Knowledge used
- OOP (for Component)
- Using .h and .cpp logically.
- Math
## Bugs
- If you see any bugs in our game, [`contact us`](https://forms.gle/8Jtg4FsLFkTioYHx9).
## Self-Assessment
- 8-8.5
