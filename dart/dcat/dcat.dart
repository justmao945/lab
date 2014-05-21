import 'dart:async';
import 'dart:convert';
import 'dart:io';

import 'package:args/args.dart';

const LINE_NUMBER = 'line-number';
var NEWLINE = '\n';

ArgResults argResults;

void main(List<String> arguments) {
  final parser = new ArgParser()
    ..addFlag(LINE_NUMBER, negatable:false, abbr:'n');

  argResults = parser.parse(arguments);
  List<String> paths = argResults.rest;

  dcat(paths, argResults[LINE_NUMBER]);
}

Future dcat(List<String> paths, bool showLineNumber) {
  if(paths.isEmpty) {
    return stdin.pipe(stdout);
  } else {
    return Future.forEach(paths, (path) {
      int lineNumber = 1;
      Stream<List<int>> stream = new File(path).openRead();
      return stream
        .transform(UTF8.decoder)
        .transform(const LineSplitter())
        .listen((line) {
          if(showLineNumber) {
            stdout.write('${lineNumber++} ');
          }
          stdout.writeln(line);
        })
        .asFuture().catchError((_) {
          FileSystemEntity.isDirectory(path).then((isDir) {
            if(isDir) {
              print('error: $path is a directory');
            } else {
              print('error: $path not found');
            }
          });
          exitCode = 2;
        });
    });
  }
}
