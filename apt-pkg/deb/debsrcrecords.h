// -*- mode: cpp; mode: fold -*-
// Description								/*{{{*/
// $Id: debsrcrecords.h,v 1.8 2004/03/17 05:58:54 mdz Exp $
/* ######################################################################
   
   Debian Source Package Records - Parser implementation for Debian style
                                   source indexes
   
   ##################################################################### */
									/*}}}*/
#ifndef PKGLIB_DEBSRCRECORDS_H
#define PKGLIB_DEBSRCRECORDS_H

#include <apt-pkg/srcrecords.h>
#include <apt-pkg/tagfile.h>
#include <apt-pkg/fileutl.h>

#include <stddef.h>
#include <string>
#include <vector>

class pkgIndexFile;

class APT_HIDDEN debSrcRecordParser : public pkgSrcRecords::Parser
{
   /** \brief dpointer placeholder (for later in case we need it) */
   void * const d;

 protected:
   FileFd Fd;
   pkgTagFile Tags;
   pkgTagSection Sect;
   std::vector<const char*> StaticBinList;
   unsigned long iOffset;
   char *Buffer;
   
   public:

   virtual bool Restart() APT_OVERRIDE {return Jump(0);};
   virtual bool Step() APT_OVERRIDE {iOffset = Tags.Offset(); return Tags.Step(Sect);};
   virtual bool Jump(unsigned long const &Off) APT_OVERRIDE {iOffset = Off; return Tags.Jump(Sect,Off);};

   virtual std::string Package() const APT_OVERRIDE {return Sect.FindS("Package");};
   virtual std::string Version() const APT_OVERRIDE {return Sect.FindS("Version");};
   virtual std::string Maintainer() const APT_OVERRIDE {return Sect.FindS("Maintainer");};
   virtual std::string Section() const APT_OVERRIDE {return Sect.FindS("Section");};
   virtual const char **Binaries() APT_OVERRIDE;
   virtual bool BuildDepends(std::vector<BuildDepRec> &BuildDeps, bool const &ArchOnly, bool const &StripMultiArch = true) APT_OVERRIDE;
   virtual unsigned long Offset() APT_OVERRIDE {return iOffset;};
   virtual std::string AsStr() APT_OVERRIDE 
   {
      const char *Start=0,*Stop=0;
      Sect.GetSection(Start,Stop);
      return std::string(Start,Stop);
   };
   virtual bool Files(std::vector<pkgSrcRecords::File> &F) APT_OVERRIDE;
   bool Files2(std::vector<pkgSrcRecords::File2> &F);

   debSrcRecordParser(std::string const &File,pkgIndexFile const *Index);
   virtual ~debSrcRecordParser();
};

class APT_HIDDEN debDscRecordParser : public debSrcRecordParser
{
 public:
   debDscRecordParser(std::string const &DscFile, pkgIndexFile const *Index);
};

#endif
