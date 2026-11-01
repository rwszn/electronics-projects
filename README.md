# Electronics Projects

A public collection of electronics builds, including project guides, wiring diagrams, source code, and build notes.

This repository is intended for viewers and other makers. It contains project-specific information needed to understand and recreate each build. It does not contain a personal parts inventory, private working notes, AI-assistant context, or private project-management instructions.

## Repository layout

Each public project has its own folder inside `projects/`:

```text
projects/
  project-name/
    README.md
    code/
      project-name-1.0.ino
      project-name-1.1.ino
    schematics/
      wiring-diagram.svg
    photos/
      ...
```

Only include files that are useful and safe to publish.

## Project documentation

Each project README should explain:
- What the project does.
- Its current published version.
- The project-specific components required.
- Wiring and connections.
- Required software and libraries.
- How to assemble, upload, and use it.
- Known limitations and testing status.
- A short version history.

A project parts list describes what that particular build needs. It is not a list of the repository owner's personal stock or available inventory.

## Code versioning

1. The first published working version is numbered `1.0`.
2. Save each code update as a new, incremented version file. Do not silently replace an older published version.
3. Keep the latest three version files in each project's `code/` folder.
4. When a fourth version is added, remove the oldest version file from the folder.
5. Update the project README with the current version and a short change history.
6. Keep wiring diagrams and instructions consistent with the published code.

## Privacy

Do not publish personal contact details, home/location information, private inventory records, credentials, tokens, Wi-Fi passwords, private AI instructions, or unrelated personal notes. Check photos and screenshots for identifying information before adding them.

## Accuracy

Clearly distinguish tested builds from untested examples. Never claim that code has been compiled or hardware-tested unless that has actually been done.
