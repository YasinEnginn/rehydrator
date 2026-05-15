# Security Policy

## Supported Versions

This is a coursework firmware adaptation project, so there are no long-term
supported release branches. Security fixes should target the current `main`
branch unless a tagged release explicitly says otherwise.

## Reporting a Vulnerability

If you find a problem that could affect flashing safety, boot flow integrity, or
device configuration, open a private report if the repository host supports it.
If private reporting is not available, open an issue with only the minimum
technical detail needed to reproduce the problem and avoid sharing secrets or
device-specific credentials.

## Firmware Safety Notes

- Do not commit generated `.hex`, `.bin`, `.elf`, `.out`, `.map`, or `build/`
  artifacts to the repository.
- Avoid full chip erase while testing this project; the BIM and CCFG flash area
  must be preserved unless you intentionally reprogram it.
- Verify generated release artifacts with checksums before sharing them.
