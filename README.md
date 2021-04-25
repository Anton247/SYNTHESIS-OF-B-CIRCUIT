Для любого множества функциональных зависимостей F, заданного на непустом множестве атрибутов R = {A<sub>1</sub>, A<sub>2</sub>, ..., A<sub>3</sub>}, 
всегда существует В-схема 	&#961;<sub>s</sub> = {R<sub>1</sub>, R<sub>2</sub>, ..., R<sub>m</sub>}, т. е. схема, обладающая свойствами сохранения F, 
соединения без потерь информации относительно F и находящаяся в 3НФ. Такую схему можно построить с помощью следующего алгоритма синтеза.

1. Вначале для исходного множества F ≠ &empty; строится редуцированное
покрытие (1):
F<sub>ред</sub> = {X<sub>1</sub> → Y<sub>1</sub>, X<sub>2</sub> → Y<sub>2</sub>, ..., X<sub>m</sub> → Y<sub>m</sub>}. 		(1)
 Это действие гарантирует 3НФ для искомой В-схемы &#961;<sub>s</sub>
 
2. Затем проверяется условие: существует в F<sub>ред</sub> зависимость X<sub>i</sub> → Y<sub>i</sub> такая, что X<sub>i</sub> &cup; Y<sub>i</sub> = R? 
Если такая зависимость существует, то процесс построения В-схемы завершается и &#961;<sub>s</sub> = {R}.
3.  Если проверяемое условие неверно, то осуществляется разложение R по ФЗ из F<sub>ред</sub> следующим образом (2): 
&#961;<sub>s</sub> = {R<sub>1</sub>(X<sub>1</sub>Y<sub>1</sub>), R<sub>2</sub>(X<sub>2</sub>Y<sub>2</sub>), ..., R<sub>m</sub>(X<sub>m</sub>Y<sub>m</sub>)}, (2)
т. е. по каждой X<sub>i</sub> → Y<sub>i</sub> &isin; F<sub>ред</sub> создается подсхема R<sub>i</sub> = X<sub>i</sub>Y<sub>i</sub>
4. Далее выполняется проверка свойства соединения без потерь информации относительно  F<sub>ред</sub>. Если свойство соединения
без потерь информации имеет место, то процесс завершается и &#961;<sub>s</sub> - требуемая В-схема
5. Если свойство соединения без потерь не выполняется, то осуществляется поиск ключа K для отношения r(R) путем добавления к  F<sub>ред</sub> фиктивной 
функциональной зависимости и редуцирования полученного множества.
6.  Искомая В-схема определяется в этом случае так (3):
&#961;<sub>s</sub> = {R<sub>1</sub>(X<sub>1</sub>Y<sub>1</sub>), R<sub>2</sub>(X<sub>2</sub>Y<sub>2</sub>), ..., R<sub>m</sub>(X<sub>m</sub>Y<sub>m</sub>),R<sub>m+1</sub>(K)}, (3)
т. е. к &#961;<sub>s</sub> добавляется подсхема, содержащая все атрибуты найденного ключа K. Это гарантирует выполнение свойства соединения без потерь информации относительно F 

>В алгоритме синтеза следует учитывать следующие очевидные факты. При F =&empty;  всегда &#961;<sub>s</sub> = {R} есть В-схема. 
Применение к  F<sub>ред</sub> правила вывода F<sub>3</sub> уменьшает число подсхем в &#961;<sub>s</sub>, оставляя ее В-схемой.

[Пособие](https://cloud.mail.ru/public/JH9p/BuhBazw7u "Пособие")
