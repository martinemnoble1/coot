

/ .*_.*chooserdialog.* = gtk_file_chooser_dialog_new / { 
  # let's get rid of this message:
  # awk: fix-filechooser-sentinels.awk:4: warning: escape sequence `\)' treated as plain `)'
  # gsub("NULL\);", "NULL, NULL);");
  gsub("NULL);", "NULL, NULL);");
  print $0;
  next;
}

{ print $0;}

