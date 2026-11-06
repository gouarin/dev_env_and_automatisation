# Mise en place de la session pratique

:::{hint}Objectifs de cette partie
- Présenter le projet
- Créer l'environnement de développement avec pixi
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

Les dépendances du projet sont

- [xtensor](https://github.com/xtensor-stack/xtensor) qui permet de manipuler des conteneurs comme NumPy mais en C++.
- [OpenCV](https://github.com/opencv/opencv) pour afficher l'image.
- [cli11](https://github.com/CLIUtils/CLI11) pour créer rapidement des analyseurs de ligne de commande.

Le projet ne décrit pas encore son environnement de développement : c'est la première chose que vous allez faire.

## Initialisation du projet

Nous allons commencer par mettre l'ensemble des fichiers se trouvant dans le répertoire `material/0.initialization/required_files` dans un répertoire que vous aurez créé et qui se trouve en dehors du répertoire git que vous venez de cloner.

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

Vous devrez ensuite configurer votre dépôt GitHub comme indiqué dans [Configurer son dépôt GitHub](depot).

## Environnement de développement

Vous allez maintenant décrire avec pixi tout ce qu'il faut pour compiler, lancer et tester `splinart-cpp`. Si vous avez besoin d'un rappel sur les commandes, relisez la section [](#pixi).

Dans le répertoire du projet, initialisez pixi

```bash
pixi init
```

pixi crée un fichier `pixi.toml` qui ne contient que votre plateforme. L'intégration continue tournera plus tard sous Linux, macOS et Windows : ajoutez ces plateformes dès maintenant pour que le fichier `pixi.lock` les couvre aussi.

```bash
pixi workspace platform add linux-64 osx-arm64 win-64
```

Ajoutez ensuite les dépendances avec `pixi add`

- pour compiler : `cxx-compiler`, `cmake` et `ninja` ;
- pour le code : `xtensor`, `opencv` et `cli11` ;
- pour les tests : `gtest`.

Les chapitres suivants utilisent aussi des outils de formatage, d'analyse statique et de documentation. Ajoutez-les maintenant pour ne plus avoir à y revenir : `clang-format`, `clang-tools`, `pre-commit`, `sphinx`, `pydata-sphinx-theme`, `doxygen`, `breathe` et `myst-parser`.

Il reste à définir les tâches dans la section `[tasks]` de `pixi.toml`. Elles passent toutes par CMake avec le générateur Ninja et construisent dans le répertoire `.build`.

- `configure` configure le projet en mode `Release` ;
- `build` configure puis compile ;
- `start` compile puis lance l'exécutable `.build/splinart-cpp` ;
- `configure_test` et `build_test` font la même chose avec l'option `-DBUILD_TESTS=ON` ;
- `test` compile les tests puis lance `.build/tests/test_splinart`.

:::{tip}
Une tâche peut dépendre d'autres tâches grâce à `depends-on`. Par exemple

```toml
build = { depends-on = ["configure", "build_ninja"] }
```
:::

Pour vérifier que tout fonctionne, lancez l'application

```bash
pixi run start
```

Une fenêtre doit s'ouvrir avec un cercle comme celui présenté plus haut. Lancez ensuite les tests

```bash
pixi run test
```

Quand tout fonctionne, ajoutez `pixi.toml` et `pixi.lock` dans un commit et poussez-le sur votre dépôt.

:::{note}
Une correction de cette étape se trouve dans le répertoire `material/1.pixi/solution`. Si vous êtes bloqué, vous pouvez copier ses deux fichiers dans votre projet.
:::

Vous êtes prêt pour la suite !

:::{important}
La correction de chacune des étapes peut se retrouver dans le répertoire `material`.
:::
