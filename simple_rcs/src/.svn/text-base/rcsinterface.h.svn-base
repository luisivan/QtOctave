#ifndef __RCSINTERFACE_H__
#define __RCSINTERFACE_H__

#include <QStringList>
#include <QHash>

class RcsInterface
{
	
	protected:
	
	QString repository;
	
	public:
	
	/**Add new version of file to repository.
	*/
	virtual bool add(QString file)=0;
	
	/**Extract file from repository.
	*/
	virtual bool extract(QString file, QString version)=0;
	
	/**Properties of version.
	*/
	virtual QString properties(QString version)=0;
	
	/**Diferences between versions.
	*/
	virtual QString diff(QString version1, QString version2)=0;
	
	/**List of versions in repository.
	*/
	virtual QStringList versions()=0;
	
	/**Set comment of version.
	 * @return true if action could be possible.
	 */
	virtual bool setComment(QString version, QString comment)=0;
	
	/**Get comment of version.
	 */
	virtual QHash<QString, QString> getComment()=0;
	
	inline void setRepository(QString repo) {repository=repo;};
};

#endif
