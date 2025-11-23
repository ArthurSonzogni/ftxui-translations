# Entrée Tuberée POSIX dans FTXUI

> [!WARNING]
> Cette fonctionnalité ne fonctionne que sur Linux et macOS. Elle n'est pas prise en charge sur
> Windows et WebAssembly.

## Qu'est-ce qu'un Tube POSIX ?

Un tube POSIX est un moyen pour deux programmes distincts de communiquer. Un programme envoie sa sortie directement comme entrée à un autre programme. Pensez-y comme un tuyau unidirectionnel pour les données.

**Exemple :**

Imaginez que vous voulez lister des fichiers et les filtrer ensuite interactivement.

- `ls` : Liste les fichiers.
- `interactive_grep` : Une application FTXUI qui filtre le texte et vous permet de taper.

Vous pouvez les connecter avec un tube (`|`) :

```bash
ls -l | interactive_grep
```

Voici ce qui se passe :
1. `ls -l` liste les fichiers avec les détails.
2. Le `|` envoie cette liste directement à `interactive_grep`.
3. `interactive_grep` reçoit la liste et l'affiche. Parce que c'est une application FTXUI, vous pouvez ensuite taper pour filtrer la liste, même si elle a reçu des données initiales de `ls`.

## Comment FTXUI gère l'entrée tuberée

Maintenant que vous comprenez ce qu'est un tube POSIX, examinons comment FTXUI les utilise.

FTXUI permet à votre application de lire des données d'autres programmes (comme depuis un tube) tout en vous permettant d'utiliser votre clavier pour l'interaction. Ceci est utile pour les outils interactifs en ligne de commande qui traitent des données.

Normalement, les applications FTXUI reçoivent toutes les entrées de `stdin`. Cependant, lorsque FTXUI détecte que `stdin` est connecté à la sortie d'un tube (ce qui signifie que des données sont tuberées dans votre application), il bascule automatiquement vers la lecture de l'entrée clavier interactive depuis `/dev/tty`. Cela garantit que votre application peut toujours recevoir des entrées utilisateur même en traitant des données tuberées.

Cette fonctionnalité est **activée par défaut**.

Si votre application FTXUI a besoin de lire des données tuberées et également de répondre aux entrées clavier, vous n'avez généralement rien de spécial à faire :

```cpp
auto screen = ScreenInteractive::Fullscreen();
// screen.HandlePipedInput(true); // Ceci est activé par défaut
screen.Loop(component);
```

## Désactiver l'entrée tuberée

Si vous n'avez pas besoin de cette fonctionnalité, ou si elle entre en conflit avec votre gestion personnalisée des entrées, vous pouvez la désactiver.

Pour la désactiver, appelez `HandlePipedInput(false)` avant de démarrer la boucle principale de votre application :

```cpp
auto screen = ScreenInteractive::Fullscreen();
screen.HandlePipedInput(false); // Désactiver la gestion de l'entrée tuberée
screen.Loop(component);
```