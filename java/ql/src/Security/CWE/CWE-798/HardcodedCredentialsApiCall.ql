/**
 * @name Hard-coded credential in API call
 * @description Using a hard-coded credential in a call to a sensitive Java API may compromise security.
 * @kind problem
 * @problem.severity error
 * @precision medium
 * @id java/hardcoded-credential-api-call
 * @tags security
 *       external/cwe/cwe-798
 */

import java
import semmle.code.java.dataflow.DataFlow
import HardcodedCredentials

class HardcodedCredentialApiCallConfiguration extends DataFlow::Configuration {
  HardcodedCredentialApiCallConfiguration() { this = "HardcodedCredentialApiCallConfiguration" }

  override predicate isSource(DataFlow::Node n) {
    n.asExpr() instanceof HardcodedExpr and
    not n.asExpr().getEnclosingCallable().getName() = "toString"
  }

  override predicate isSink(DataFlow::Node n) { n.asExpr() instanceof CredentialsApiSink }

  override predicate isAdditionalFlowStep(DataFlow::Node node1, DataFlow::Node node2) {
    node1.asExpr().getType() instanceof TypeString and
    exists(MethodAccess ma | ma.getMethod().getName().regexpMatch("getBytes|toCharArray") |
      node2.asExpr() = ma and
      ma.getQualifier() = node1.asExpr()
    )
  }
}

from CredentialsApiSink sink, HardcodedExpr source, HardcodedCredentialApiCallConfiguration conf
where conf.hasFlow(DataFlow::exprNode(source), DataFlow::exprNode(sink))
select source, "Hard-coded value flows to $@.", sink, "sensitive API call"
