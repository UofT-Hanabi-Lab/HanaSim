# Contributing to HanaSim

If you are contributing to this project, please follow the guidelines detailed below to ensure a smooth contribution process.

## Branching strategy

- We prefer using a feature-based branching strategy. When creating a new feature, create a feature branch:
    ```
    git checkout -b feat-feature-name
    ```
    Ensure that the name of your branch starts with `feat-`

- If the feature is large and requires contributions by more than one student, create a user branch off the feature branch like so
    ```
    git checkout -b feat-feature-name/your-name
    ```

## Pull Request Guidelines
- Ensure your branch is up to date whenever you submit a pull request to avoid merge conflicts.
- Consider using `git rebase` and `git push --force` when your local branch diverges from the remote branch. (Only use `git push --force` on local branches).
- Always use the Pull Request Template when creating a pull request, and if applicable, link related issues in the PR description.
- It is good practice to commit frequently during development and squash commits where appropriate before making a pull request. All changes related to a feature or fix should be combined into a single commit, making it easier to track, review, and revert if necessary.
- After you submit a PR, make sure to request a review from other contributors. You can do so on the right side of the screen by adding a username under "Reviewers". This is important because a review is required before merging.

