//invoke with cqlsh -f db_create.cql
CREATE KEYSPACE dev WITH replication = { 'class' : 'SimpleStrategy', 'replication_factor' : 1 };
use dev;

CREATE TABLE Secrets(
  Type text,
  Name text,
  Deleted Boolean,
  AlgorithmType text,
  SecretRef text,
  OrderRef text,
  ID text,
  Description text,
  State int,
  Tags list<text>,
  Crn text,
  StorageLocation text,
  StorageType text,
  LinkedResource text,
  RelatedResources list<text>,
  EscrowedBy text,

  //Audit Trail
  CreatedBy text,
  DeletedBy text,
  LastUpdatedBy text,

  //Crypto Period
  ActivationDate timestamp,
  CreationDate timestamp,
  DeletionDate timestamp,
  ExpirationDate timestamp,
  LastUpdateDate timestamp,

  AlgorithmMetadata map<text, text>,
  UserMetadata map<text, text>,

  Space Text,
  GUID UUID PRIMARY KEY
);

CREATE INDEX ON Secrets (Space);
