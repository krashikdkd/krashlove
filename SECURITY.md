# Security Policy

## Supported Versions

Currently supported versions of KrashLove:

| Version | Supported          |
| ------- | ------------------ |
| 1.0.x   | :white_check_mark: |

## Security Considerations

KrashLove is a system optimization tool that requires administrator privileges. This inherently carries security implications:

### By Design
- **Administrator Access**: Required for system-level operations
- **Registry Modifications**: Changes TCP/IP and CPU settings
- **Service Management**: Can disable/enable Windows services
- **File System Access**: Deletes temporary files and prefetch data
- **Memory Operations**: Modifies process working sets

### Safety Measures
- **No Network Communication**: Tool operates entirely offline
- **No Data Collection**: No telemetry or user data gathering
- **Conservative Defaults**: Only targets known safe operations
- **Reversible Changes**: Most optimizations can be undone
- **User Confirmation**: Prompts before making significant changes
- **Error Handling**: Graceful failure without system corruption

## Reporting a Vulnerability

If you discover a security vulnerability in KrashLove, please follow responsible disclosure:

1. **DO NOT** open a public issue
2. Email the maintainers privately (see GitHub profile)
3. Include:
   - Description of the vulnerability
   - Steps to reproduce
   - Potential impact
   - Suggested fix (if any)

### Response Timeline
- **Initial Response**: Within 48 hours
- **Status Update**: Within 7 days
- **Fix Timeline**: Depends on severity
  - Critical: 24-48 hours
  - High: 7 days
  - Medium: 14 days
  - Low: 30 days

## Known Security Considerations

### Administrator Privileges
The tool requires admin rights. Users should:
- Only download from official sources
- Verify file integrity
- Run only when needed
- Review code if concerned

### Registry Modifications
Changes to registry keys are permanent until manually reverted:
- TCP/IP settings
- CPU core parking
- Power management

**Recommendation**: Create a system restore point before running.

### Service Management
Disabling services can affect system functionality:
- Telemetry services (safe to disable)
- Xbox services (safe if not gaming)
- Other services (manually reviewed)

**Recommendation**: Note which services are disabled for easy restoration.

### File Deletions
Temporary file cleanup is generally safe but:
- Files in use are skipped
- Some temp files may be needed by apps
- Recycle bin is permanently emptied

**Recommendation**: Close all applications before cleanup.

## Best Practices for Users

1. **Backup First**
   - Create system restore point
   - Backup important data
   - Note current system state

2. **Review Changes**
   - Understand what each option does
   - Start with individual optimizations
   - Monitor system behavior

3. **Safe Environment**
   - Run on test system first
   - Avoid running on production servers
   - Keep other security software active

4. **Verification**
   - Download only from official GitHub releases
   - Verify checksums if provided
   - Review code if technically capable

## Antivirus False Positives

Some antivirus software may flag KrashLove due to:
- System-level operations
- Registry modifications
- Service management
- Memory operations

This is a **false positive**. The tool:
- Contains no malware
- Makes no network connections
- Collects no data
- Is open source

## Building from Source

For maximum security, build from source:
```bash
git clone https://github.com/krashikdkd/krashlove.git
cd krashlove
# Review the code
mkdir build && cd build
cmake ..
cmake --build . --config Release
```

## Disclaimer

KrashLove is provided "as is" without warranty. Users assume all risks. The developers are not liable for:
- Data loss
- System instability
- Performance issues
- Any other damages

**Use at your own risk. Always backup your system.**

## Questions?

For security-related questions (non-vulnerabilities), open a GitHub issue with the `security` label.
