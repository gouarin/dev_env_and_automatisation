# Mise en place de la session pratique

:::{hint}Objectifs de cette partie
- Présenter le projet
- Initialiser l'environnement de développement
:::

Dans le premier chapitre, nous avons fait le tour des outils à mettre en place pour définir le cadre de développement, favoriser et faciliter les interactions entre les différents acteurs gravitant autour d'un projet open source. Nous en avons présenté certain plus en détail.

Pour la suite, nous supposerons que

- vous avez installé pixi sur votre machine. Si ce n'est pas le cas, veuillez vous rendre à la section décrivant [](#pixi).
- vous avez cloné le répertoire https://github.com/gouarin/dev_env_and_automatisation comprenant ce document, mais aussi le matériel pour la session pratique.

Nous allons à présent décrire l'ensemble des étapes nécessaires pour vous simplifier la vie et celle de vos utilisateurs et contributeurs.

## Présentation du projet

Nous allons nous appuyer sur un code C++ déjà écrit et prêt à l'emploi qui fait du [generative art](https://en.wikipedia.org/wiki/Generative_art) à partir de l'équation d'un cercle et de splines. Ce programme affiche des rendus de ce type

:::{figure} ./figures/splinart.png
:::

L'idée vient du site [inconvergent](https://inconvergent.net/generative/sand-spline/).

Dans ce projet, l'arborescence est la suivante

:::{figure} ./figures/splinart-tree.png
:::

- Les répertoires `include` et `src` contiennent les sources de l'application.
- Le répertoire `examples` donne un exemple très simple d'utilisation.
- Le répertoire `tests` teste l'application en utilisant [googletest](https://github.com/google/googletest).
- Le fichier `LICENSE` décrit une licence BSD.
- Les fichiers `pixi*` décrivent l'environnement de développement.

Les dépendances du projet sont

- [xtensor](https://github.com/xtensor-stack/xtensor) qui permet de manipuler des conteneurs comme NumPy mais en C++.
- [OpenCV](https://github.com/opencv/opencv) pour afficher l'image.
- [cli11](https://github.com/CLIUtils/CLI11) pour créer rapidement des analyseurs de ligne de commande.

Ces dépendances sont décrites dans le fichier `pixi.toml`.

Nous avons également ajouté un certain nombre de tâches pour pouvoir construire et tester rapidement l'application.

## Initialisation du projet

Nous allons commencer par mettre l'ensemble des fichiers se trouvant dans le répertoire `material/1.initialization/required_files` dans un répertoire que vous aurez crée et qui se trouve en dehors du répertoire git que vous venez de cloner.

Vous pouvez maintenant initialiser le système de gestion de version pour ce répertoire en faisant

```bash
git init
```

Puis ajoutez l'ensemble des fichiers et faites votre premier commit.

## Initialisation du dépôt distant

Vous allez à présent créer un dépôt sur votre compte GitHub. Nous vous proposons de l'appeler `splinart-cpp`. Une fois que c'est fait, il ne vous reste plus qu'à ajouter le dépôt distant dans votre répertoire en suivant les instructions proposées par GitHub.

:::{note}
Si vous ne savez pas comment faire : [création d'un dépôt](https://docs.github.com/repositories/creating-and-managing-repositories/creating-a-new-repository).
:::

Vous devrez ensuite configurer votre dépôt GitHub comme indiquer dans [Configurer son dépôt GitHub](depot).

## Première exécution

Vous pouvez à présent créer l'environnement de développement sous pixi dans le répertoire que vous venez de créer à l'aide de la ligne de commande

```bash
pixi install
```

Enfin, pour vérifier que tout fonctionne correctement, il vous suffit de faire

```bash
pixi run start
```

Vous devriez voir apparaître un cercle comme présenté ci-dessus.



Vous êtes prêt pour la suite !


:::{important}
La correction de chacune des étapes peut se retrouver dans le répertoire `material`.
:::
