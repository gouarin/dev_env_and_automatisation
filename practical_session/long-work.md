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
      - uses: actions/checkout@v3

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
      - uses: actions/checkout@v3

      - id: new_commits
        run: echo "nb_commits=$(git log --oneline --since '24 hours ago' | wc -l)" >> $GITHUB_OUTPUT
```

`id` permet de faire référence au travail dans d'autres sections comme c'est le cas dans `outputs`. `GITHUB_OUTPUT` est l'endroit où vous mettez ce que vous souhaitez exporter sous forme de clé/valeur. La section `outputs` définit ce qui sera réellement exporté et accessible par les autres travaux.

## Mise en place du linter

Nous avons fait le plus difficile. Il nous reste à présent à créer le travail qui se déclenche uniquement si le nombre de commits est strictement positif en utilisant `needs`, `if` et la variable `nb_commits`. Ca donne

```yaml
  clang-tidy-check:
    needs: check_new_commit
    if: needs.check_new_commit.outputs.nb_commits > 0
    runs-on: ubuntu-latest
    steps:
      - uses: actions/checkout@v3

      - uses: prefix-dev/setup-pixi@v0.8.1
        with:
          pixi-version: v0.40.2
          cache: true

      - name: Configure
        shell: pixi run bash {0}
        run: |
          cmake . -Bbuild -DCMAKE_EXPORT_COMPILE_COMMANDS=ON -DCMAKE_BUILD_TYPE=Debug -DBUILD_EXAMPLES=ON
          pip install compdb
          compdb -p build list > compile_commands.json

      - name: run clang-tidy
        shell: pixi run bash {0}
        run: |
          run-clang-tidy
```

:::{attention}
Vous aurez également besoin de mettre à la racine du projet le fichier `.clang-tidy` qui définit la configuration souhaitée. Ce fichier se trouve dans le répertoire `material/5.long-run/required_files/`.
:::

La partie `Configure` est propre au projet. Il nous faut un certain nombre d'informations que nous pouvons retrouver via CMake et l'option `CMAKE_EXPORT_COMPILE_COMMANDS` pour pouvoir analyser le code source. Malheureusement, cette commande de donne que les commande de compilation pour les bibliothèques et les exécutables. Imaginez que vous avez un projet [header only](https://en.wikipedia.org/wiki/Header-only) et le fichier `compile_commands.json` généré par la commande CMake sera vide. C'est pour cette raison que nous utilisons un autre outil ([compdb](https://github.com/Sarcasm/compdb)) qui nous permet d'ajouter les fichiers `hpp`.

:::{note}Remarques
- Nous n'avons pas installé `clang-tidy` car il est déjà installé par `pixi` via le paquet `clang-tools`.
- `run-clang-tidy` fait partie de ce paquet. Il permet d'exécuter `clang-tidy` à partir du fichier `compile_commands.json`.
- Nous n'avons pas parlé de la configuration, car elle était déjà dans le dépôt initial (voir le fichier `.clang-tidy` à la racine du projet).
:::

Ajoutez l'ensemble de ces sections et testez votre workflow.

Vérifiez que tout fonctionne bien, puis faites un `Squash and merge`.
