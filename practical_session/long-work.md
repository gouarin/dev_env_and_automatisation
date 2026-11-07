# Travaux longs

:::{hint}Objectifs de cette partie
- Savoir comment gérer les tests longs
- Déclencher des tests à intervalles réguliers
- Mise en place de l'analyse statique via `clang-tidy`
:::

:::{note}Notions GitHub Actions abordées
- Utiliser l'événement cron
- Mettre des données dans la sortie d'un travail
- Se servir des sorties d'un autre travail
:::

Lorsque nous mettons en place l'intégration continue, il n'est pas rare d'avoir un ensemble de tests qui prend du temps. Se pose alors la question de savoir si nous infligeons aux contributeurs d'attendre plus d'une heure pour savoir si leur pull request est conforme ou si nous limitons le temps de réponse. Attendre une heure n'est bien évidemment pas raisonnable.

Il nous faut donc faire un choix sur les tests qui ne prennent pas trop de temps, mais qui nous permettent de nous assurer de la consistance du projet. Nous ne souhaitons pas pour autant oublier les autres tests. Mais, nous pourrions les faire à un autre moment.

Nous proposons la chose suivante
- les courts tests se font à chaque pull request
- les longs tests se font toutes les nuits, mais uniquement si la branche principale a été modifiée.

Dans notre exemple, nous voudrions utiliser l'outil [clang-tidy](https://clang.llvm.org/extra/clang-tidy/) pour vérifier la syntaxe du code et valider les noms des fonctions, classes... comme décrit dans [](#l-analyse-statique). Malheureusement, ce test peut prendre un certain temps. Nous allons donc le lancer en dehors de tout événement de pull request ou de push.

Nous allons continuer à approfondir les possibilités de la section `on` qui déclenche les workflows selon des événements définis. Dans GitHub Actions, il est possible d'utiliser un [cron](https://pubs.opengroup.org/onlinepubs/9699919799/utilities/crontab.html#tag_20_25_07) via l'événement [`schedule`](https://docs.github.com/actions/using-workflows/events-that-trigger-workflows#schedule).

Nous ne rentrerons pas ici dans les détails de la construction du cron. Il vous suffit de cliquer sur le lien donné plus haut pour en comprendre le fonctionnement.

:::{attention}
Sur un dépôt public, GitHub désactive automatiquement un workflow déclenché par `schedule` après 60 jours sans activité dans le dépôt. Le workflow ne tourne alors plus du tout, même sur les pull requests. Vous pouvez le réactiver dans l'onglet `Actions` du dépôt, ou avec la commande `gh workflow enable`.
:::

:::{note}
N'oubliez pas de retourner dans la branche `main` et de faire

```bash
git pull
```
:::

Créez une nouvelle branche où vous ajouterez le workflow `long_run.yml`.

Dans ce workflow, nous voulons

- le lancer à un certain moment,
- ne travailler que sous linux,
- appliquer `clang-tidy` sur les fichiers C++ du dépôt.

:::{tip} Commande cron
Voici la commande cron qui permet d'exécuter le workflow à 14h23 tous les jours

```text
'23 14 * * *'
```
:::


## Calcul du nombre de commits

Nous souhaitons déclencher l'analyse statique du code via `clang-tidy` sur la branche `main` uniquement s'il y a eu des commits depuis 24h. L'idée étant à la fin de mettre un cron qui se déclenche toutes les nuits à minuit via `'0 0 * * *'`.

Voici un exemple de travail qui calcule le nombre de commits depuis 24h à partir du moment où l'action est exécutée

```yaml
  check_new_commit:
    runs-on: ubuntu-latest
    steps:
      - uses: actions/checkout@v7

      - name: new_commits
        run: echo "nb_commits=$(git log --oneline --since '24 hours ago' | wc -l)"
```

Nous voudrions récupérer la valeur `nb_commits` et pouvoir l'utiliser pour déclencher le travail sur `clang-tidy`.

GitHub Actions propose de définir des sorties : [Defining outputs for jobs](https://docs.github.com/actions/using-jobs/defining-outputs-for-jobs). Voici ce que ça pourrait donner

```yaml
  check_new_commit:
    runs-on: ubuntu-latest
    outputs:
      nb_commits: ${{ steps.new_commits.outputs.nb_commits }}
    steps:
      - uses: actions/checkout@v7

      - id: new_commits
        run: echo "nb_commits=$(git log --oneline --since '24 hours ago' | wc -l)" >> $GITHUB_OUTPUT
```

`id` permet de faire référence au travail dans d'autres sections comme c'est le cas dans `outputs`. `GITHUB_OUTPUT` est l'endroit où vous mettez ce que vous souhaitez exporter sous forme de clé/valeur. La section `outputs` définit ce qui sera réellement exporté et accessible par les autres travaux.

## Mise en place du linter

Nous avons fait le plus difficile. Ce travail a besoin de deux outils dont le reste du projet se passe :

- `clang` fournit les en-têtes internes du compilateur (comme `stddef.h`). Sans lui, `clang-tidy` ne trouve pas ces fichiers sous Linux et s'arrête en erreur. Prenez la même version que le paquet `clang-tools` de votre `pixi.toml`.
- [compdb](https://github.com/Sarcasm/compdb), que nous présentons plus bas. Il n'existe pas sur conda-forge, nous le prenons donc sur PyPI.

Plutôt que de les ajouter à l'environnement de tout le projet, nous allons créer avec pixi un environnement réservé à l'analyse statique. Ajoutez à votre `pixi.toml`

```toml
[feature.lint.dependencies]
clang = ">=21.1,<22"

[feature.lint.pypi-dependencies]
compdb = ">=0.2.0,<0.3"

[environments]
default = { solve-group = "default" }
lint = { features = ["lint"], solve-group = "default" }
```

puis lancez `pixi lock` pour mettre à jour `pixi.lock`.

- Une *feature* regroupe des dépendances (ici `lint`). Elle n'est installée que dans les environnements qui la demandent.
- L'environnement `lint` contient les dépendances du projet et celles de la feature `lint`. L'environnement `default` reste inchangé.
- Le `solve-group` commun impose les mêmes versions dans les deux environnements : `clang-tidy` analyse le code avec exactement les bibliothèques utilisées pour le compiler.

:::{tip}
Vous pouvez obtenir le même résultat en ligne de commande

```bash
pixi add --feature lint "clang>=21.1,<22"
pixi add --feature lint --pypi compdb
pixi workspace environment add lint --feature lint --solve-group default
pixi workspace environment add default --solve-group default --force
```
:::

Pour lancer une commande dans cet environnement, il suffit d'ajouter `-e lint` : `pixi run -e lint run-clang-tidy`.

Il reste à créer le travail qui se déclenche uniquement si le nombre de commits est strictement positif en utilisant `needs`, `if` et la variable `nb_commits`. Ça donne

```yaml
  clang-tidy-check:
    needs: check_new_commit
    if: needs.check_new_commit.outputs.nb_commits > 0
    runs-on: ubuntu-latest
    steps:
      - uses: actions/checkout@v7

      - uses: prefix-dev/setup-pixi@v0.11.3
        with:
          pixi-version: v0.84.1
          environments: lint
          cache: true

      - name: Configure
        shell: pixi run -e lint bash {0}
        run: |
          cmake . -Bbuild -DCMAKE_EXPORT_COMPILE_COMMANDS=ON -DCMAKE_BUILD_TYPE=Debug -DBUILD_EXAMPLES=ON
          compdb -p build list > compile_commands.json

      - name: run clang-tidy
        shell: pixi run -e lint bash {0}
        run: |
          run-clang-tidy -extra-arg=-isystem$CONDA_PREFIX/include
```

:::{attention}
Vous aurez également besoin de mettre à la racine du projet le fichier `.clang-tidy` qui définit la configuration souhaitée. Ce fichier se trouve dans le répertoire `material/5.long-run/required_files/`.
:::

La partie `Configure` est propre au projet. Il nous faut un certain nombre d'informations que nous pouvons retrouver via CMake et l'option `CMAKE_EXPORT_COMPILE_COMMANDS` pour pouvoir analyser le code source. Malheureusement, cette commande ne donne que les commandes de compilation pour les bibliothèques et les exécutables. Imaginez que vous avez un projet [header only](https://en.wikipedia.org/wiki/Header-only) et le fichier `compile_commands.json` généré par la commande CMake sera vide. C'est pour cette raison que nous utilisons compdb, qui ajoute les fichiers `hpp`.

:::{note}Remarques
- Nous n'avons pas installé `clang-tidy` car il est déjà installé par `pixi` via le paquet `clang-tools`.
- `run-clang-tidy` fait partie de ce paquet. Il permet d'exécuter `clang-tidy` à partir du fichier `compile_commands.json`.
- Le compilateur gcc de conda-forge trouve tout seul les en-têtes installés dans l'environnement (xtensor, OpenCV...). CMake ne les écrit donc pas dans `compile_commands.json`, et `clang-tidy` ne les trouverait pas. L'option `-extra-arg=-isystem$CONDA_PREFIX/include` les lui indique : `CONDA_PREFIX` est le chemin de l'environnement pixi actif.
- Nous ne détaillons pas la configuration contenue dans le fichier `.clang-tidy`. Ouvrez-le pour voir les règles activées.
:::

Ajoutez l'ensemble de ces sections et testez votre workflow.

:::{note}
Le premier passage de `clang-tidy` échoue : le code ne respecte pas encore les règles de `.clang-tidy`, par exemple le nom de la fonction `drawPiXel` ou des variables qui pourraient être `const`. C'est le rôle de cet outil. Corrigez les erreurs signalées jusqu'à ce que le travail passe. Une correction se trouve dans `material/5.long-run/solution`.
:::

Vérifiez que tout fonctionne bien, puis faites un `Squash and merge`.
